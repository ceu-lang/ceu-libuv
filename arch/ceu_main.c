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

typedef struct {
    void* vec;
    int   has_pending_data;
} ceu_uv_read_t;

#include "_ceu_app.h"

tceu_app CEU_APP;
#if 0
uv_loop_t ceu_uv_loop;
#endif

/* FS */

/* TCP */

#define ceu_uv_tcp_connect(a,b,c) uv_tcp_connect(a,b,c,ceu_uv_connect_cb)

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

#ifdef CEU_IN_UV_WRITE
#endif

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

#if 0
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
#endif

/* THREADS */

#ifdef CEU_THREADS
uv_prepare_t ceu_uv_prepare;
uv_check_t   ceu_uv_check;
void ceu_uv_prepare_cb (uv_prepare_t* prepare) {
    ceu_threads_gc(&CEU_APP, 0);
    CEU_THREADS_MUTEX_UNLOCK(&CEU_APP.threads_mutex);
    if (CEU_APP.threads_head != NULL) {
        CEU_THREADS_SLEEP(1000);
    }
}
void ceu_uv_check_cb (uv_check_t* check) {
    CEU_THREADS_MUTEX_LOCK(&CEU_APP.threads_mutex);
}
#endif

#include "_ceu_app.c"

static char CEU_DATA[sizeof(CEU_Main)];

int main (int argc, char *argv[])
{
#if 0
    signal(SIGPIPE, SIG_IGN); // TODO: fails on "uv_write" would crash otherwise

    uv_loop_init(&ceu_uv_loop);
#endif
#ifdef CEU_ASYNCS
    uv_idle_init(&ceu_uv_loop, &ceu_uv_idle);
#endif
#if 0
#ifdef CEU_WCLOCKS
    uv_timer_init(&ceu_uv_loop, &ceu_uv_timer);
#endif
#endif
#ifdef CEU_THREADS
    uv_prepare_init(&ceu_uv_loop, &ceu_uv_prepare);
    uv_check_init(&ceu_uv_loop, &ceu_uv_check);
    assert(uv_prepare_start(&ceu_uv_prepare, ceu_uv_prepare_cb) == 0);
    assert(uv_check_start(&ceu_uv_check, ceu_uv_check_cb) == 0);
#endif

    CEU_APP.data  = (tceu_org*) &CEU_DATA;
    CEU_APP.init  = &ceu_app_init;
    CEU_APP.close = &ceu_app_close;
    CEU_APP.init(&CEU_APP);    /* calls CEU_THREADS_MUTEX_LOCK() */
#ifdef CEU_THREADS
    CEU_THREADS_MUTEX_UNLOCK(&CEU_APP.threads_mutex);
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
        CEU_THREADS_MUTEX_LOCK(&CEU_APP.threads_mutex);
#endif
        ceu_sys_go(&CEU_APP, CEU_IN_OS_START, &arg);
#ifdef CEU_THREADS
        CEU_THREADS_MUTEX_UNLOCK(&CEU_APP.threads_mutex);
#endif
    }
#ifdef CEU_RET
    if (!CEU_APP.isAlive) {
        uv_stop(&ceu_uv_loop);
    }
#endif
#endif

#ifdef CEU_THREADS
    CEU_THREADS_MUTEX_LOCK(&CEU_APP.threads_mutex);
#endif

#if 0
    // TODO: detect/error if return w/o "escape"
    uv_run(&ceu_uv_loop, UV_RUN_DEFAULT);
#endif

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

    return CEU_APP.close(&CEU_APP);
}

