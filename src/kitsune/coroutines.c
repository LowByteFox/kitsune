#include <kitsune/allocator.h>
#include <assert.h>
#include <kitsune/numbers.h>
#include <kitsune/coroutines.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

#define JOB_CHUNK 16

static struct kitsune_runtime *glob_rt = NULL;

static isize    kitsune_runtime_choose_task(struct kitsune_runtime*);
static void     kitsune_runtime_task_entry(struct kitsune_runtime*, usize);
static void     kitsune_runtime_stack_overflow(struct kitsune_runtime*);

static usize    kitsune_runtime_capacity(struct kitsune_runtime*);
static void     kitsune_runtime_reserve(struct kitsune_runtime*, usize);
static void     kitsune_runtime_shrink(struct kitsune_runtime*);

enum kitsune_task_stats {
        TASK_EMPTY = 0,
        TASK_CREATED = 1,
        TASK_RUNNING = 2,
        TASK_DONE = 3,
};

enum kitsune_runtime_stats {
        RT_RUNNING = 0x1000000,
        RT_READY = 0x2000000,
        RT_INIT = 0,
        RT_SCHEDULE = 1,
        RT_EXIT_TASK = 2,
};

struct kitsune_task {
        struct kitsune_runtime *rt;
        enum kitsune_task_stats status;
        enum kitsune_task_stats await_status;
        usize id;
        void *stack;
        void *stack_start;
        usize stack_usage;
        async void *(*fn)(void*);
        void *arg;
        void *retval;
        ucontext_t ctx;
};

struct kitsune_runtime {
        struct kitsune_allocator *allocator;
        ucontext_t ctx;
        isize current;
        usize count;
        enum kitsune_runtime_stats flags;
        usize stack_size;
        /* max is not needed */
        struct kitsune_task *tasks;
};

struct kitsune_runtime*
kitsune_runtime_init(usize stack_size, struct kitsune_allocator *allocator)
{
        struct kitsune_runtime *rt = allocator->alloc(allocator, sizeof(*rt));
        rt->allocator = allocator;

        rt->current = -1;
        rt->count = 0;
        rt->flags = (-1 & 0x00FFFFFF) | RT_READY;
        rt->stack_size = (stack_size + 4095) & (~4095);
        rt->tasks = allocator->alloc(allocator,
            JOB_CHUNK * sizeof(struct kitsune_task));

        usize i = 0;
        for (; i < JOB_CHUNK; i++) {
                rt->tasks[i].status = TASK_EMPTY;
                rt->tasks[i].await_status = TASK_EMPTY;
                rt->tasks[i].id = i;
        }

        return rt;
}

void
kitsune_runtime_deinit(struct kitsune_runtime *rt)
{
        if (rt == NULL) {
                if (glob_rt == NULL)
                        return;

                rt = glob_rt;
        }
        /* XXX: To be implemented */
        rt->allocator->free(rt->allocator, rt->tasks);
        rt->allocator->free(rt->allocator, rt);
}

void
kitsune_runtime_run(struct kitsune_runtime *rt)
{
        if (rt == NULL) {
                if (glob_rt == NULL)
                        return;

                rt = glob_rt;
        }

        rt->flags |= RT_RUNNING;
        rt->current = -1;

        do {
            rt->current = kitsune_runtime_choose_task(rt);
            if (rt->current >= 0) {
                    struct kitsune_task *task = rt->tasks + rt->current;
                    swapcontext(&rt->ctx, &task->ctx);
                    if (task->status == TASK_DONE) {
                            task->status = TASK_EMPTY;
                            rt->count--;
                            rt->allocator->free(rt->allocator,
                                task->ctx.uc_stack.ss_sp);

                            usize capacity = kitsune_runtime_capacity(rt);
                            if (rt->count + JOB_CHUNK == capacity)
                                    kitsune_runtime_shrink(rt);
                    }
            }
        } while (rt->current >= 0);
        rt->flags &= ~RT_RUNNING;
}

void
kitsune_runtime_terminate(struct kitsune_runtime *rt)
{
        if (rt == NULL) {
                if (glob_rt == NULL)
                        return;

                rt = glob_rt;
        }

        if (rt->flags & RT_RUNNING)
                return;

        if (rt->tasks != NULL)
                rt->allocator->free(rt->allocator, rt->tasks);

        rt->tasks = NULL;
        return;
}

struct kitsune_task*
kitsune_runtime_create_task(struct kitsune_runtime *rt,
    async void *(*fn)(void*), void *arg)
{
        if (rt == NULL) {
                if (glob_rt == NULL)
                        return NULL;

                rt = glob_rt;
        }

        usize capacity = kitsune_runtime_capacity(rt);
        if (rt->count == capacity) {
                kitsune_runtime_reserve(rt, capacity + JOB_CHUNK);
                capacity = capacity + JOB_CHUNK;
        }
        usize i = 0;
        while (i < capacity && rt->tasks[i].status != TASK_EMPTY) i++;
        if (i >= capacity) return NULL;

        struct kitsune_task *task = rt->tasks + i;
        task->rt = rt;
        task->status = TASK_CREATED;
        task->await_status = TASK_CREATED;
        task->stack = rt->allocator->alloc(rt->allocator, rt->stack_size);
        task->retval = NULL;
        task->stack_usage = 0;
        task->id = i;
        task->fn = fn;
        task->arg = arg;

        if (getcontext(&task->ctx) < 0) {
                task->status = TASK_EMPTY;
                rt->allocator->free(rt->allocator, task->stack);
        }
        task->ctx.uc_stack.ss_sp = task->stack;
        task->ctx.uc_stack.ss_size = rt->stack_size;
        task->ctx.uc_link = NULL;
        makecontext(&task->ctx, (void(*)()) kitsune_runtime_task_entry, 2,
            rt, task->id);

        rt->count++;
        return task;
}

void
kitsune_runtime_yield(struct kitsune_runtime *rt)
{
        if (rt == NULL) {
                if (glob_rt == NULL)
                        return;

                rt = glob_rt;
        }

        kitsune_runtime_stack_overflow(rt);
        swapcontext(&rt->tasks[rt->current].ctx, &rt->ctx);
}

usize
kitsune_runtime_ntasks(struct kitsune_runtime *rt)
{
        if (rt == NULL) {
                if (glob_rt == NULL)
                        return 0;

                rt = glob_rt;
        }
        return rt->count;
}

usize
kitsune_task_id(struct kitsune_task *task)
{
        return task->id;
}

void*
kitsune_task_await(struct kitsune_task *task)
{
        while (task->await_status != TASK_DONE) {
                kitsune_runtime_yield(task->rt);
        }

        return task->retval;
}

void
kitsune_set_global_runtime(struct kitsune_runtime *rt)
{
        glob_rt = rt;
}

static isize
kitsune_runtime_choose_task(struct kitsune_runtime *rt)
{
        struct kitsune_task* task = NULL;

        if (rt->count == 0) return -1;

        isize current = 0;
        usize capacity = kitsune_runtime_capacity(rt);
        if (rt->current >= 0)
                current = (rt->current + 1) % capacity;

        usize i = 0;
        for (; i < capacity; i++) {
                task = rt->tasks + current;
                if (task->status == RT_RUNNING ||
                    task->status == TASK_CREATED)
                        return current;
                current = (current + 1) % capacity;
        }

        return -1;
}

static void
kitsune_runtime_task_entry(struct kitsune_runtime *rt, usize id)
{
        struct kitsune_task *task = rt->tasks + id;
        
        task->stack_start = &task;
        task->retval = task->fn(task->arg);
        task->status = TASK_DONE;
        task->await_status = TASK_DONE;
        swapcontext(&rt->tasks[rt->current].ctx, &rt->ctx);
}

static void
kitsune_runtime_stack_overflow(struct kitsune_runtime *rt)
{
        struct kitsune_task *task = rt->tasks + rt->current;
        usize size = ((usize) task->stack_start - (usize) &task);
        size = (size < 0) ? -size : size;

        if (size > rt->stack_size - 32) {
                fprintf(stderr, "*** stack overflow at task #%ld\n",
                    rt->current);
                fprintf(stderr, "\tstack size: %ld\n", rt->stack_size);
                fprintf(stderr, "\tstack usage: %ld\n", size);
                abort();
        }

        task->stack_usage = size;
}

static usize
kitsune_runtime_capacity(struct kitsune_runtime *rt)
{
        usize allocated = kitsune_allocated(rt->tasks);
        return allocated / sizeof(struct kitsune_task);
}

static void
kitsune_runtime_reserve(struct kitsune_runtime *rt, usize new_capacity)
{
        usize capacity = kitsune_runtime_capacity(rt);
        if (new_capacity <= capacity)
                return;

        rt->tasks = rt->allocator->resize(rt->allocator, rt->tasks,
            new_capacity * sizeof(struct kitsune_task));
}

static void
kitsune_runtime_shrink(struct kitsune_runtime *rt)
{
        usize allocated = kitsune_allocated(rt->tasks);
        if (allocated == rt->count * sizeof(struct kitsune_task))
                return;

        rt->tasks = rt->allocator->resize(rt->allocator, rt->tasks,
            rt->count * sizeof(struct kitsune_task));
}
