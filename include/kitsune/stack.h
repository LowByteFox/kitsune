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

#ifndef _STACK_H_
#define _STACK_H_

#include <kitsune/allocator.h>
#include <kitsune/numbers.h>

struct kitsune_stack {
        struct kitsune_allocator *allocator;
        usize chunksize;
        void *items;
        usize size;
};

struct kitsune_stack    kitsune_stack_init(usize, struct kitsune_allocator*);
void                    kitsune_stack_deinit(struct kitsune_stack*,
                            kitsune_allocator_deletor*);
void                    kitsune_stack_push(struct kitsune_stack*, void*);
/*
 * POP ALLOCATES memory for the removed element
 * Don't forget to free it!
 */
void                   *kitsune_stack_pop(struct kitsune_stack*);
void                   *kitsune_stack_top(struct kitsune_stack*);
usize                   kitsune_stack_size(struct kitsune_stack*);
bool                    kitsune_stack_empty(struct kitsune_stack*);

#endif
