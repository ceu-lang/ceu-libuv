#ifndef _CEU_THREADS_H
#define _CEU_THREADS_H

#include <unistd.h>     /* usleep */

#define CEU_THREADS_T               uv_thread_t
#define CEU_THREADS_MUTEX_T         uv_mutex_t
#define CEU_THREADS_SELF()          uv_thread_self()
#define CEU_THREADS_CREATE(t,f,p)   uv_thread_create(t,f,p)
#define CEU_THREADS_CANCEL(t)
#define CEU_THREADS_DETACH(t)
#define CEU_THREADS_MUTEX_LOCK(m)   uv_mutex_lock(m)
#define CEU_THREADS_MUTEX_UNLOCK(m) uv_mutex_unlock(m)
#define CEU_THREADS_SLEEP(us)       usleep(us)
#define CEU_THREADS_PROTOTYPE(f,p)  void f (p)
#define CEU_THREADS_RETURN(v)

#endif
