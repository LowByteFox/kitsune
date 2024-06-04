#ifndef _COROUTINES_H_
#define _COROUTINES_H_

#include <allocator.h>
#include <numbers.h>

#define async
#define yield kitsune_runtime_yield((void*) 0);
#define await kitsune_task_await

struct kitsune_runtime;
struct kitsune_task;

struct kitsune_runtime     *kitsune_runtime_init(usize,
                                struct kitsune_allocator*);
void                        kitsune_runtime_deinit(struct kitsune_runtime*);
void                        kitsune_runtime_run(struct kitsune_runtime*);
void                        kitsune_runtime_terminate(struct kitsune_runtime*);
struct kitsune_task        *kitsune_runtime_create_task(
                                struct kitsune_runtime*,
                                async void *(*)(void*), void*);
void                        kitsune_runtime_yield(struct kitsune_runtime*);
usize                       kitsune_runtime_ntasks(struct kitsune_runtime*);
usize                       kitsune_task_id(struct kitsune_task*);
void                       *kitsune_task_await(struct kitsune_task*);

#ifdef ENABLE_RT
void                        kitsune_set_global_runtime(struct kitsune_runtime*);
#endif

#endif
