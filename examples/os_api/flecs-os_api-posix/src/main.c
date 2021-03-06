#include <flecs_os_api_posix.h>

#include "pthread.h"

static
ecs_os_thread_t posix_thread_new(
    ecs_os_thread_callback_t callback, 
    void *arg)
{
    pthread_t thread;
    int r;

    if ((r = pthread_create (&thread, NULL, callback, arg))) {
        abort();
    }

    return (ecs_os_thread_t)thread;
}

static
void* posix_thread_join(
    ecs_os_thread_t thread)
{
    void *arg;
    pthread_join((pthread_t)thread, &arg);
    return arg;
}

static
int32_t posix_ainc(int32_t *count) {
    int value;
#ifdef __GNUC__
    value = __sync_add_and_fetch (count, 1);
    return value;
#else
    /* Unsupported */
    abort();
#endif
}

static
int32_t posix_adec(int32_t *count) {
    int value;
#ifdef __GNUC__
    value = __sync_sub_and_fetch (count, 1);
    return value;
#else
    /* Unsupported */
    abort();
#endif
}

static
ecs_os_mutex_t posix_mutex_new(void) {
    pthread_mutex_t *mutex = ecs_os_malloc(sizeof(pthread_mutex_t));
    if (pthread_mutex_init(mutex, NULL)) {
        abort();
    }
    return (ecs_os_mutex_t)(uintptr_t)mutex;
}

static
void posix_mutex_free(ecs_os_mutex_t m) {
    pthread_mutex_t *mutex = (pthread_mutex_t*)(intptr_t)m;
    pthread_mutex_destroy(mutex);
    ecs_os_free(mutex);
}

static
void posix_mutex_lock(ecs_os_mutex_t m) {
    pthread_mutex_t *mutex = (pthread_mutex_t*)(intptr_t)m;
    if (pthread_mutex_lock(mutex)) {
        abort();
    }
}

static
void posix_mutex_unlock(ecs_os_mutex_t m) {
    pthread_mutex_t *mutex = (pthread_mutex_t*)(intptr_t)m;
    if (pthread_mutex_unlock(mutex)) {
        abort();
    }
}

static
ecs_os_cond_t posix_cond_new(void) {
    pthread_cond_t *cond = ecs_os_malloc(sizeof(pthread_cond_t));
    if (pthread_cond_init(cond, NULL)) {
        abort();
    }
    return (ecs_os_cond_t)(uintptr_t)cond;
}

static 
void posix_cond_free(ecs_os_cond_t c) {
    pthread_cond_t *cond = (pthread_cond_t*)(intptr_t)c;
    if (pthread_cond_destroy(cond)) {
        abort();
    }
    ecs_os_free(cond);
}

static 
void posix_cond_signal(ecs_os_cond_t c) {
    pthread_cond_t *cond = (pthread_cond_t*)(intptr_t)c;
    if (pthread_cond_signal(cond)) {
        abort();
    }
}

static 
void posix_cond_broadcast(ecs_os_cond_t c) {
    pthread_cond_t *cond = (pthread_cond_t*)(intptr_t)c;
    if (pthread_cond_broadcast(cond)) {
        abort();
    }
}

static 
void posix_cond_wait(ecs_os_cond_t c, ecs_os_mutex_t m) {
    pthread_cond_t *cond = (pthread_cond_t*)(intptr_t)c;
    pthread_mutex_t *mutex = (pthread_mutex_t*)(intptr_t)m;
    if (pthread_cond_wait(cond, mutex)) {
        abort();
    }
}

void posix_set_os_api(void) {
    ecs_os_set_api_defaults();

    ecs_os_api_t api = ecs_os_api;

    api._thread_new = posix_thread_new;
    api._thread_join = posix_thread_join;
    api._ainc = posix_ainc;
    api._adec = posix_adec;
    api._mutex_new = posix_mutex_new;
    api._mutex_free = posix_mutex_free;
    api._mutex_lock = posix_mutex_lock;
    api._mutex_unlock = posix_mutex_unlock;
    api._cond_new = posix_cond_new;
    api._cond_free = posix_cond_free;
    api._cond_signal = posix_cond_signal;
    api._cond_broadcast = posix_cond_broadcast;
    api._cond_wait = posix_cond_wait;

    ecs_os_set_api(&api);
}

