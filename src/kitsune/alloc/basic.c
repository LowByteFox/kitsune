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

#include <kitsune/allocator.h>
#include <kitsune/alloc/basic.h>
#include <kitsune/numbers.h>

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

static void *
allocate(struct kitsune_allocator *a, usize size)
{
        (void) a;

        struct kitsune_pointer *ptr = calloc(1, size + sizeof(usize));
        if (ptr == NULL)
                return ptr;

        ptr->size = size;
        return ptr->ptr;
}

static void *
reallocate(struct kitsune_allocator *a, void *ptr, usize size)
{
        (void) a;

        struct kitsune_pointer *orig = NULL;

        if (ptr != NULL)
                orig = kitsune_visualize(ptr);

        orig = realloc(orig, sizeof(struct kitsune_pointer) + size);
        if (orig == NULL)
                return NULL;

        orig->size = size;
        
        return orig->ptr;
}

static void
destroy(struct kitsune_allocator *a, void *ptr)
{
        (void) a;

        if (ptr == NULL)
                return;
        struct kitsune_pointer *orig = kitsune_visualize(ptr);
        free(orig);
}

struct kitsune_allocator *const kitsune_basic_allocator =
    (struct kitsune_allocator[]) { (struct kitsune_allocator) {
    .alloc = allocate, .resize = reallocate, .free = destroy, 
    .context = NULL } };
