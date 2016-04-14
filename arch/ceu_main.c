#include <assert.h>
#include <stdio.h>
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

s32 WCLOCK_nxt;
#define ceu_out_wclock_set(us) WCLOCK_nxt = us;

tceu_app CEU_APP;
uv_loop_t ceu_uv_loop;

#define ceu_uv_fs_open(a,b,c,d)   uv_fs_open(&ceu_uv_loop,a,b,c,d,ceu_uv_fs_cb)
#define ceu_uv_fs_read(a,b,c,d,e) uv_fs_read(&ceu_uv_loop,a,b,c,d,e,ceu_uv_fs_cb)
#define ceu_uv_fs_close(a,b)      uv_fs_close(&ceu_uv_loop,a,b,ceu_uv_fs_close_cb)

void ceu_uv_fs_cb (uv_fs_t* req) {
#ifdef CEU_IN_FS
    ceu_sys_go(&CEU_APP, CEU_IN_FS, &req);
#endif
}

void ceu_uv_fs_close_cb (uv_fs_t* req) {
    assert(req->result == 0);
    uv_fs_req_cleanup(req);
}

#include "_ceu_app.c"

static char CEU_DATA[sizeof(CEU_Main)];

int main (int argc, char *argv[])
{
    uv_loop_init(&ceu_uv_loop);

    CEU_APP.data = (tceu_org*) &CEU_DATA;
    CEU_APP.init = &ceu_app_init;
    CEU_APP.init(&CEU_APP);    /* calls CEU_THREADS_MUTEX_LOCK() */
#ifdef CEU_RET
    if (! CEU_APP.isAlive)
        goto END;
#endif

#ifdef CEU_IN_OS_START_
    ceu_sys_go(&CEU_APP, CEU_IN_OS_START_, NULL);
#ifdef CEU_RET
    if (! CEU_APP.isAlive)
        goto END;
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

