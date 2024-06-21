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

#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <kitsune/numbers.h>

/* Structural visualization of kitsune pointer */
struct kitsune_pointer {
        usize size;
        u8 ptr[];
};

struct kitsune_allocator {
        void *(*alloc)(struct kitsune_allocator *const, usize);
        void *context;
        void (*free)(struct kitsune_allocator *const, void*);
        void *(*resize)(struct kitsune_allocator *const, void*, usize);
};

typedef void kitsune_allocator_deletor(struct kitsune_allocator*, void*);

void   *kitsune_allocator_alloc(struct kitsune_allocator *const, usize);
void   *kitsune_allocator_resize(struct kitsune_allocator *const, void*, 
            usize);
void    kitsune_allocator_free(struct kitsune_allocator *const, void*);
usize   kitsune_allocated(const void*);

struct kitsune_pointer *kitsune_visualize(void*);

#endif
