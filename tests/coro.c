#include <numbers.h>
#include <coroutines.h>

#include <allocator.h>
#include <alloc/basic.h>
#include <stdio.h>


/*
 * This is bad practice!
 */
struct kitsune_allocator *a;

struct task_data {
        const char *name;
        usize iter_count;
};

async void
process(const char *name, usize i)
{
        printf("Task with name %s at iter %ld\n", name, i);
}

async void*
task_fn(void *arg)
{
        struct task_data *data = arg;
        usize sum = 0;
        usize i = 0;

        for (; i < data->iter_count; i++) {
            sum += i + 1;
            process(data->name, i);
            yield;
        }
        a->free(a, data);

        usize *ret = a->alloc(a, sizeof(usize));
        *ret = sum;
        return ret;
}

async struct kitsune_task*
create_task(const char *name, usize iter)
{
        struct task_data *data = a->alloc(a, sizeof(*data));
        data->name = name;
        data->iter_count = iter;

        return kitsune_runtime_create_task(NULL, task_fn, data);
}

async i32
async_main()
{
        a = kitsune_basic_allocator;
        struct kitsune_task *task = create_task("Alpha", 3);
        struct kitsune_task *task2 = create_task("Beta", 5);
        usize* data = kitsune_task_await(task);
        printf("Result sum is: %ld\n", *data);

        data = kitsune_task_await(task2);
        printf("Result sum is: %ld\n", *data);

        task = create_task("Gamma", 10);
        data = kitsune_task_await(task);
        printf("Result sum is: %ld\n", *data);

        return 0;
}
