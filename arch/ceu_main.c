#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#define ceu_out_assert(v) ceu_sys_assert(v)
void ceu_sys_assert (int v) {
    assert(v);
}

#define ceu_out_log(m,s) ceu_sys_log(m,s)
void ceu_sys_log (int mode, long s) {
    switch (mode) {
        case 0:
            fprintf(stderr, "%s", (char*)s);
            break;
        case 1:
            fprintf(stderr, "%lX", s);
            break;
        case 2:
            fprintf(stderr, "%ld", s);
            break;
    }
}

#include "_ceu_app.h"

tceu_app CEU_APP;
uv_loop_t ceu_uv_loop;

/* FS */

#define ceu_uv_fs_open(a,b,c,d)   uv_fs_open(&ceu_uv_loop,a,b,c,d,ceu_uv_fs_cb)
#define ceu_uv_fs_read(a,b,c,d,e) uv_fs_read(&ceu_uv_loop,a,b,c,d,e,ceu_uv_fs_cb)
#define ceu_uv_fs_close(a,b)      uv_fs_close(&ceu_uv_loop,a,b,ceu_uv_fs_close_cb)

void ceu_uv_fs_cb (uv_fs_t* req) {
#ifdef CEU_IN_FS
    ceu_sys_go(&CEU_APP, CEU_IN_FS, &req);
#endif
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
}

void ceu_uv_fs_close_cb (uv_fs_t* req) {
    assert(req->result == 0);
    uv_fs_req_cleanup(req);
}

/* TCP */

#define ceu_uv_tcp_init(a)   uv_tcp_init(&ceu_uv_loop, a);
#define ceu_uv_listen(a,b)   uv_listen(a,b,ceu_uv_listen_cb)
#define ceu_uv_read_start(a) uv_read_start(a,ceu_uv_malloc,ceu_uv_read_start_cb)
#define ceu_uv_write(a,b,c)  uv_write(a,b,c,1,ceu_uv_write_cb)

void ceu_uv_listen_cb (uv_stream_t *s, int status) {
#ifdef CEU_IN_UV_LISTEN
    tceu__uv_stream_t___int p = { s, status };
    ceu_sys_go(&CEU_APP, CEU_IN_UV_LISTEN, &p);
#endif
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
}

void ceu_uv_malloc (uv_handle_t* h, size_t size, uv_buf_t* buf) {
    buf->base = (char*) malloc(size);
    buf->len = size;
}

void ceu_uv_free (uv_handle_t* h) {
    free(h);
}

void ceu_uv_read_start_cb(uv_stream_t* s, ssize_t n, const uv_buf_t* buf) {
#ifdef CEU_IN_UV_READ
    tceu__uv_stream_t___ssize_t__uv_buf_t_ p = { s, n, (uv_buf_t*)buf };
    ceu_sys_go(&CEU_APP, CEU_IN_UV_READ, &p);
#endif
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
}

void ceu_uv_write_cb (uv_write_t* req, int status) {
#ifdef CEU_IN_UV_WRITE
    tceu__uv_write_t___int p = { req, status };
    ceu_sys_go(&CEU_APP, CEU_IN_UV_WRITE, &p);
#endif
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
}

/* ASYNCS */

#ifdef CEU_ASYNCS
uv_idle_t ceu_uv_idle;
#define ceu_out_async(_) uv_idle_start(&ceu_uv_idle, ceu_uv_idle_cb);
void ceu_uv_idle_cb (uv_idle_t* idler) {
    assert(idler == &ceu_uv_idle);
    ceu_sys_go(&CEU_APP, CEU_IN__ASYNC, NULL);
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
    if (!CEU_APP.pendingAsyncs) {
        uv_idle_stop(idler);
    }
}
#endif

/* WCLOCKS */

#ifdef CEU_WCLOCKS
uv_timer_t ceu_uv_timer;
double ceu_uv_uptime;
#define ceu_out_wclock_set(us)                                      \
    if (us == CEU_WCLOCK_INACTIVE) {                                \
         uv_timer_stop(&ceu_uv_timer);                              \
    } else {                                                        \
        uv_timer_start(&ceu_uv_timer, ceu_uv_timer_cb, us/1000, 0); \
    }
void ceu_uv_timer_cb (uv_timer_t* timer) {
    assert(timer == &ceu_uv_timer);
    double now;
    uv_uptime(&now);
    int dt = (now - ceu_uv_uptime)*1000000;
    ceu_uv_uptime = now;
    ceu_sys_go(&CEU_APP, CEU_IN__WCLOCK, &dt);
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
}
#endif

#include "_ceu_app.c"

static char CEU_DATA[sizeof(CEU_Main)];

int main (int argc, char *argv[])
{
    uv_loop_init(&ceu_uv_loop);
#ifdef CEU_ASYNCS
    uv_idle_init(&ceu_uv_loop, &ceu_uv_idle);
#endif
#ifdef CEU_WCLOCKS
    uv_uptime(&ceu_uv_uptime);
    uv_timer_init(&ceu_uv_loop, &ceu_uv_timer);
#endif

    CEU_APP.data = (tceu_org*) &CEU_DATA;
    CEU_APP.init = &ceu_app_init;
    CEU_APP.init(&CEU_APP);    /* calls CEU_THREADS_MUTEX_LOCK() */
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif

#ifdef CEU_IN_OS_START_
    ceu_sys_go(&CEU_APP, CEU_IN_OS_START_, NULL);
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
#endif

    uv_run(&ceu_uv_loop, UV_RUN_DEFAULT);

END:
    uv_loop_close(&ceu_uv_loop);
#ifdef CEU_RET
    return CEU_APP.ret;
#else
    return 0;
#endif
}

