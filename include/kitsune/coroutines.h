/*
 * Copyright (C) 2024 LowByteFox
 * This file is part of kitsune.
 *
 * kitsune is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3.0 of the License, or
 * (at your option) any later version.
 *
 * kitsune is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with kitsune; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _COROUTINES_H_
#define _COROUTINES_H_

#include <kitsune/allocator.h>
#include <kitsune/numbers.h>

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

void                        kitsune_set_global_runtime(struct kitsune_runtime*);

#endif
