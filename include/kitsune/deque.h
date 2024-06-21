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

#ifndef _DEQUE_H_
#define _DEQUE_H_

#include <kitsune/dynamic_iterator.h>
#include <kitsune/vec.h>
#include <kitsune/allocator.h>
#include <kitsune/numbers.h>

struct kitsune_deque {
        struct kitsune_allocator *allocator;
        struct kitsune_vec front;
        struct kitsune_vec back;
        usize size;
};

struct kitsune_deque    kitsune_deque_init(usize, struct kitsune_allocator*);
void                    kitsune_deque_deinit(struct kitsune_deque*,
                            kitsune_allocator_deletor*);
void                    kitsune_deque_push_back(struct kitsune_deque*, void*);
void                    kitsune_deque_push_front(struct kitsune_deque*, void*);
void                    kitsune_deque_insert(struct kitsune_deque*, usize,
                            void*);
void                   *kitsune_deque_back(struct kitsune_deque*);
void                   *kitsune_deque_front(struct kitsune_deque*);
/*
 * BOTH POP AND REMOVE return ALLCATED memory by the deque
 * Don't forget to free it!
 */
void                   *kitsune_deque_pop_back(struct kitsune_deque*);
void                   *kitsune_deque_pop_front(struct kitsune_deque*);
void                   *kitsune_deque_remove(struct kitsune_deque*, usize);
void                   *kitsune_deque_get(struct kitsune_deque*, usize);
usize                   kitsune_deque_size(struct kitsune_deque*);
bool                    kitsune_deque_empty(struct kitsune_deque*);

/* Iterator API */
struct kitsune_dynamic_iterator kitsune_deque_iterator(struct kitsune_deque*);
struct kitsune_dynamic_iterator kitsune_deque_reverse_iterator(
                                    struct kitsune_deque*);

#endif
