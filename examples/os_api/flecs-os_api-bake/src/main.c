#include <flecs_os_api_bake.h>

static
ecs_os_thread_t bake_thread_new(
    ecs_os_thread_callback_t callback, 
    void *param)
{
    return (ecs_os_thread_t)ut_thread_new(callback, param);
}

static
void* bake_thread_join(
    ecs_os_thread_t thread)
{
    void *arg;
    ut_thread_join((ut_thread)thread, &arg);
    return arg;
}

static
int32_t bake_ainc(int32_t *value) {
    return ut_ainc(value);
}

static
int32_t bake_adec(int32_t *value) {
    return ut_adec(value);
}

static
ecs_os_mutex_t bake_mutex_new(void) {
    struct ut_mutex_s *m = ecs_os_api._malloc(sizeof(struct ut_mutex_s));
    ut_mutex_new(m);
    return (ecs_os_mutex_t)(uintptr_t)m;
}

static
void bake_mutex_free(ecs_os_mutex_t mutex) {
    ut_mutex_free((struct ut_mutex_s*)mutex);
    ecs_os_api._free((struct ut_mutex_s*)mutex);
}

static
void bake_mutex_lock(ecs_os_mutex_t mutex) {
    ut_mutex_lock((struct ut_mutex_s*)mutex);
}

static
void bake_mutex_unlock(ecs_os_mutex_t mutex) {
    ut_mutex_unlock((struct ut_mutex_s*)mutex);
}

static
ecs_os_cond_t bake_cond_new(void) {
    struct ut_cond_s *c = ecs_os_api._malloc(sizeof(struct ut_cond_s));
    ut_cond_new(c);
    return (ecs_os_cond_t)(uintptr_t)c;
}

static 
void bake_cond_free(ecs_os_cond_t cond) {
    ut_cond_free((struct ut_cond_s *)cond);
    ecs_os_api._free((struct ut_cond_s *)cond);
}

static 
void bake_cond_signal(ecs_os_cond_t cond) {
    ut_cond_signal((struct ut_cond_s *)cond);
}

static 
void bake_cond_broadcast(ecs_os_cond_t cond) {
    ut_cond_broadcast((struct ut_cond_s *)cond);
}

static 
void bake_cond_wait(ecs_os_cond_t cond, ecs_os_mutex_t mutex) {
    ut_cond_wait((struct ut_cond_s *)cond, (struct ut_mutex_s *)mutex);
}

static
ecs_os_dl_t bake_dlopen(
    const char *dlname)
{
    return (ecs_os_dl_t)ut_dl_open(dlname);
}

static
void bake_dlclose(
    ecs_os_dl_t dl)
{
    ut_dl_close((ut_dl)dl);
}

static
ecs_os_proc_t bake_dlproc(
    ecs_os_dl_t dl,
    const char *procname)
{
    ecs_os_proc_t result = (ecs_os_proc_t)ut_dl_proc((ut_dl)dl, procname);
    if (!result) {
        ut_raise();
    }
    return result;
}

static
char* bake_module_to_dl(
    const char *module_id)
{
    const char *result = ut_locate(module_id, NULL, UT_LOCATE_LIB);
    if (result) {
        return ut_strdup(result);
    } else {
        return NULL;
    }
}

static
char* bake_module_to_etc(
    const char *module_id)
{
    const char *result = ut_locate(module_id, NULL, UT_LOCATE_ETC);
    if (result) {
        return ut_strdup(result);
    } else {
        return NULL;
    }
}

static
void bake_init(void)
{
    ut_init(NULL);
    if (ut_load_init(NULL, NULL, NULL, NULL)) {
        ecs_os_err("warning: failed to initialize package loader");
    }
}

static
void bake_fini(void)
{
    ut_deinit();
}

void bake_set_os_api(void) {
    ecs_os_set_api_defaults();

    ecs_os_api_t api = ecs_os_api;

    api._init = bake_init;
    api._fini = bake_fini;
    api._thread_new = bake_thread_new;
    api._thread_join = bake_thread_join;
    api._ainc = bake_ainc;
    api._adec = bake_adec;
    api._mutex_new = bake_mutex_new;
    api._mutex_free = bake_mutex_free;
    api._mutex_lock = bake_mutex_lock;
    api._mutex_unlock = bake_mutex_unlock;
    api._cond_new = bake_cond_new;
    api._cond_free = bake_cond_free;
    api._cond_signal = bake_cond_signal;
    api._cond_broadcast = bake_cond_broadcast;
    api._cond_wait = bake_cond_wait;
    api._dlopen = bake_dlopen;
    api._dlproc = bake_dlproc;
    api._dlclose = bake_dlclose;
    api._module_to_dl = bake_module_to_dl;
    api._module_to_etc = bake_module_to_etc;

    ecs_os_set_api(&api);
}

