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
#include <kitsune/alloc/hardened.h>
#include <kitsune/numbers.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MV(ptr, op) ((u8*) ptr) op sizeof(usize)
#define CANARY_SIZE 100

static char CLEAN_CANARY[CANARY_SIZE];

static void *
allocate(struct kitsune_allocator *a, usize size)
{
        (void) a;

        struct kitsune_pointer *ptr = calloc(1, sizeof(struct kitsune_pointer)
            + size + CANARY_SIZE);
        if (ptr == NULL)
                return ptr;

        memset(ptr->ptr + ptr->size, 0, CANARY_SIZE);

        *(usize*) ptr = size;
        return MV(ptr, +);
}

static void *
reallocate(struct kitsune_allocator *a, void *ptr, usize size)
{
        (void) a;

        void *orig = NULL;

        if (ptr != NULL)
                orig = MV(ptr, -);

        orig = realloc(orig, size + sizeof(usize) + CANARY_SIZE);
        if (orig == NULL)
                return NULL;

        memset(((u8*) orig) + sizeof(usize) + size, 0, CANARY_SIZE);

        *(usize*) orig = size;
        
        return MV(orig, +);
}

static void
check_pointer(void *ptr)
{
        if (memcmp(ptr, CLEAN_CANARY, CANARY_SIZE) != 0) {
                fprintf(stderr, "*** heap smashing detected ***: terminated\n");
                abort();
        }
}

static void
destroy(struct kitsune_allocator *a, void *ptr)
{
        (void) a;

        if (ptr == NULL)
                return;

        struct kitsune_pointer *p = kitsune_visualize(ptr);
        if (p == NULL)
                return;

        check_pointer(p->ptr + p->size);

        free(p);
}

#undef MV

struct kitsune_allocator *const kitsune_hardened_allocator =
    (struct kitsune_allocator[]) { (struct kitsune_allocator) {
    .alloc = allocate, .resize = reallocate, .free = destroy, 
    .context = NULL } };
