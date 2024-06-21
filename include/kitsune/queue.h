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

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <kitsune/numbers.h>
#include <kitsune/list.h>
#include <kitsune/allocator.h>

struct kitsune_queue {
        struct kitsune_list list;
};

struct kitsune_queue    kitsune_queue_init(usize, struct kitsune_allocator*);
void                    kitsune_queue_deinit(struct kitsune_queue*,
                            kitsune_allocator_deletor*);
void                    kitsune_queue_push(struct kitsune_queue*, void*);
/*
 * POP ALLOCATES memory for the removed element
 * Don't forget to remove it
 */
void                   *kitsune_queue_pop(struct kitsune_queue*);
void                   *kitsune_queue_front(struct kitsune_queue*);
void                   *kitsune_queue_back(struct kitsune_queue*);
usize                   kitsune_queue_size(struct kitsune_queue*);
bool                    kitsune_queue_empty(struct kitsune_queue*);

#endif
