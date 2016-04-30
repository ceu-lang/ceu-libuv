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

typedef struct sockaddr         sockaddr;
typedef struct sockaddr_in      sockaddr_in;
typedef struct sockaddr_storage sockaddr_storage;
typedef struct {
    void* vec;
    int   has_pending_data;
} ceu_uv_read_t;

#include "_ceu_app.h"

tceu_app CEU_APP;
uv_loop_t ceu_uv_loop;

void ceu_uv_free (uv_handle_t* h) {
    // TODO: other stream types
#if 0
    if (h->type == UV_TCP) {
        uv_stream_t* s = (uv_stream_t*)h;
        if (s->connect_req != NULL) {
printf(">>> FREE\n");
            free(s->connect_req);
            s->connect_req = NULL;
        }
    }
#endif
    free(h);
}

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

#if 0
#define ceu_uv_close(a)           if (!uv_is_closing(a)) { uv_close(a,ceu_uv_free); }
#endif
#define ceu_uv_close(a)           uv_close(a,ceu_uv_free)
#define ceu_uv_tcp_init(a)        uv_tcp_init(&ceu_uv_loop, a);
#define ceu_uv_tcp_connect(a,b,c) uv_tcp_connect(a,b,c,ceu_uv_connect_cb)
#define ceu_uv_listen(a,b)        uv_listen(a,b,ceu_uv_listen_cb)
#define ceu_uv_read_start(a)      uv_read_start(a,ceu_uv_read_alloc,ceu_uv_read_start_cb);
#define ceu_uv_write(a,b,c)       uv_write(a,b,c,1,ceu_uv_write_cb)

void ceu_uv_connect_cb (uv_connect_t* c, int err) {
#ifdef CEU_IN_UV_CONNECT
    tceu__uv_connect_t___int p = { c, err };
    ceu_sys_go(&CEU_APP, CEU_IN_UV_CONNECT, &p);
#endif
    free(c);
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
}

void ceu_uv_listen_cb (uv_stream_t* s, int err) {
    assert(err >= 0);
#ifdef CEU_IN_UV_LISTEN
    tceu__uv_stream_t___int p = { s, err };
    ceu_sys_go(&CEU_APP, CEU_IN_UV_LISTEN, &p);
#endif
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
}

#define CEU_UV_READ_ALLOC_DYN_INIT 128

void ceu_uv_read_alloc (uv_handle_t* h, size_t size, uv_buf_t* buf) {
    assert(h->data != NULL);
    tceu_vector* vec = (tceu_vector*) h->data;

    int len = ceu_vector_getlen(vec);
    {
        // init dynamic vector or
        // grow dynamic vector if going past half of it
        int max = ceu_vector_getmax(vec);
        if (max <= 0) {
            if (max == 0) {
                ceu_vector_resize(vec, CEU_UV_READ_ALLOC_DYN_INIT);
            } else if (max<0 && len*2>(-max)) {
                ceu_vector_resize(vec, (-max+10)*3/2);
            }
        }
    }
    {
        int max = ceu_vector_getmax(vec);
        *buf = uv_buf_init(vec->mem+len, ((max<0) ? -max : max)-len);
    }
}

void ceu_uv_read_start_cb(uv_stream_t* s, ssize_t n, const uv_buf_t* buf) {
    assert(n != 0); // TODO: if this happens, try to understand why

    assert(s->data != NULL);
    tceu_vector* vec = (tceu_vector*) s->data;
    //assert(vec->mem == (byte*)buf->base);
    if (n > 0) {
        if (ceu_vector_setlen(vec, ceu_vector_getlen(vec)+n,1) == 0) {
            n = UV_ENOBUFS;
        }
        //assert(vec->mem == (byte*)buf->base);
    }

#ifdef CEU_IN_UV_ERROR
    if (n < 0) {
        tceu__uv_stream_t___int p = { s, n };
        ceu_sys_go(&CEU_APP, CEU_IN_UV_ERROR, &p);
#ifdef CEU_RET
        if (!CEU_APP.isAlive) {
            uv_stop(&ceu_uv_loop);
        }
#endif
    }
#endif

#ifdef CEU_IN_UV_READ
    {
        tceu__uv_stream_t___ssize_t__uv_buf_t_ p = { s, n, (uv_buf_t*)buf };
        ceu_sys_go(&CEU_APP, CEU_IN_UV_READ, &p);
#ifdef CEU_RET
        if (!CEU_APP.isAlive) {
            uv_stop(&ceu_uv_loop);
        }
#endif
    }
#endif
}

void ceu_uv_write_cb (uv_write_t* req, int err) {
#ifdef CEU_IN_UV_ERROR
    if (err < 0) {
        // if the "assert" fails, see if the error makes sense and extend it
        assert(err == UV__EOF);
        tceu__uv_stream_t___int p = { req->handle, err };
        ceu_sys_go(&CEU_APP, CEU_IN_UV_ERROR, &p);
    }
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
#endif
#ifdef CEU_IN_UV_WRITE
    tceu__uv_write_t___int p = { req, err };
    ceu_sys_go(&CEU_APP, CEU_IN_UV_WRITE, &p);
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
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
s32 ceu_uv_timer_next_us;
#define ceu_out_wclock_set(us)         \
    if (us == CEU_WCLOCK_INACTIVE) {   \
        uv_timer_stop(&ceu_uv_timer);  \
    } else if (us <= 0) {              \
        ceu_uv_timer_next_us = 0;      \
        uv_timer_start(&ceu_uv_timer, ceu_uv_timer_cb, 0, 0); \
    } else {                           \
        ceu_uv_timer_next_us = us;     \
        uv_timer_start(&ceu_uv_timer, ceu_uv_timer_cb, ceu_uv_timer_next_us/1000, 0); \
    }

void ceu_uv_timer_cb (uv_timer_t* timer) {
    assert(timer == &ceu_uv_timer);
    s32 now = ceu_uv_timer_next_us;
    ceu_sys_go(&CEU_APP, CEU_IN__WCLOCK, &now);
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
}
#endif

/* THREADS */

#ifdef CEU_THREADS
uv_prepare_t ceu_uv_prepare;
uv_check_t   ceu_uv_check;
void ceu_uv_prepare_cb (uv_prepare_t* prepare) {
    CEU_THREADS_MUTEX_UNLOCK(&CEU_APP.threads_mutex_external);
    if (CEU_APP.threads_n > 0) {
        //CEU_THREADS_SLEEP(1000);
    }
}
void ceu_uv_check_cb (uv_check_t* check) {
    CEU_THREADS_MUTEX_LOCK(&CEU_APP.threads_mutex_external);
}
#endif

#include "_ceu_app.c"

static char CEU_DATA[sizeof(CEU_Main)];

int main (int argc, char *argv[])
{
    signal(SIGPIPE, SIG_IGN); // TODO: fails on "uv_write" would crash otherwise

    uv_loop_init(&ceu_uv_loop);
#ifdef CEU_ASYNCS
    uv_idle_init(&ceu_uv_loop, &ceu_uv_idle);
#endif
#ifdef CEU_WCLOCKS
    uv_timer_init(&ceu_uv_loop, &ceu_uv_timer);
#endif
#ifdef CEU_THREADS
    uv_prepare_init(&ceu_uv_loop, &ceu_uv_prepare);
    uv_check_init(&ceu_uv_loop, &ceu_uv_check);
    assert(uv_prepare_start(&ceu_uv_prepare, ceu_uv_prepare_cb) == 0);
    assert(uv_check_start(&ceu_uv_check, ceu_uv_check_cb) == 0);
#endif

    CEU_APP.data = (tceu_org*) &CEU_DATA;
    CEU_APP.init = &ceu_app_init;
    CEU_APP.init(&CEU_APP);    /* calls CEU_THREADS_MUTEX_LOCK() */
#ifdef CEU_THREADS
    CEU_THREADS_MUTEX_UNLOCK(&CEU_APP.threads_mutex_external);
#endif
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif

#ifdef CEU_IN_OS_START
    {
        tceu_os_start arg = { argc, argv };
#ifdef CEU_THREADS
        CEU_THREADS_MUTEX_LOCK(&CEU_APP.threads_mutex_external);
#endif
        ceu_sys_go(&CEU_APP, CEU_IN_OS_START, &arg);
#ifdef CEU_THREADS
        CEU_THREADS_MUTEX_UNLOCK(&CEU_APP.threads_mutex_external);
#endif
    }
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
#endif

    // TODO: detect/error if return w/o "escape"
    uv_run(&ceu_uv_loop, UV_RUN_DEFAULT);

#ifdef CEU_ASYNCS
    uv_close((uv_handle_t*)&ceu_uv_idle, NULL);
#endif
#ifdef CEU_WCLOCKS
    uv_close((uv_handle_t*)&ceu_uv_timer, NULL);
#endif
#ifdef CEU_THREADS
    uv_close((uv_handle_t*)&ceu_uv_prepare, NULL);
    uv_close((uv_handle_t*)&ceu_uv_check, NULL);
#endif
    while (uv_run(&ceu_uv_loop,UV_RUN_NOWAIT) != 0);
    assert(uv_loop_close(&ceu_uv_loop) == 0);

#ifdef CEU_RET
    return CEU_APP.ret;
#else
    return 0;
#endif
}

