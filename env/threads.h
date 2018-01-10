#include <uv.h>

#include <unistd.h>     /* usleep */

#define CEU_THREADS_T               uv_thread_t
#define CEU_THREADS_MUTEX_T         uv_mutex_t
#define CEU_THREADS_SELF()          uv_thread_self()
#define CEU_THREADS_CREATE(t,f,p)   uv_thread_create(t,f,p)
#define CEU_THREADS_CANCEL(t)
#define CEU_THREADS_JOIN_TRY(t)     0
#define CEU_THREADS_JOIN(t)         ceu_assert_ex(uv_thread_join(&t)==0, "bug found", CEU_TRACE_null)
#define CEU_THREADS_MUTEX_LOCK(m)   uv_mutex_lock(m)
#define CEU_THREADS_MUTEX_UNLOCK(m) uv_mutex_unlock(m)
#define CEU_THREADS_SLEEP(us)       usleep(us)
#define CEU_THREADS_PROTOTYPE(f,p)  void f (p)
#define CEU_THREADS_RETURN(v)
