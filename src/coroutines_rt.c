#ifdef ENABLE_RT
#include <allocator.h>
#include <alloc/basic.h>
#include <coroutines.h>
#include <numbers.h>

i32 async_main(i32 argc, char **argv);

static i32 exit_code = 0;

struct main_data {
        i32 argc;
        char **argv;
};

static async void*
async_main_hook(void *arg)
{
        struct main_data *data = arg;
        exit_code = async_main(data->argc, data->argv);
        return NULL;
        
}


i32
main(i32 argc, char **argv)
{
        struct kitsune_allocator *a = kitsune_basic_allocator;
        struct kitsune_runtime *rt = kitsune_runtime_init(16 * 1024, a);
        kitsune_set_global_runtime(rt);

        struct main_data data = (struct main_data) { .argc = argc, .argv = argv };

        kitsune_runtime_create_task(rt, async_main_hook, &data);
        kitsune_runtime_run(rt);

        kitsune_runtime_deinit(rt);
        return exit_code;
}
#endif
