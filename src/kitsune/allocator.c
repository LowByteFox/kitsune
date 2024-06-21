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

void *
kitsune_allocator_alloc(struct kitsune_allocator *const a, size_t size)
{
        return a->alloc(a, size);
}

void *
kitsune_allocator_resize(struct kitsune_allocator *const a, void *ptr,
    size_t size)
{
        return a->resize(a, ptr, size);
}

void 
kitsune_allocator_free(struct kitsune_allocator *const a, void *ptr)
{
        a->free(a, ptr);
}

usize
kitsune_allocated(const void *ptr)
{
        return ptr == NULL ? 0 : *(usize*) (((u8*) ptr) - sizeof(usize));
}

struct kitsune_pointer*
kitsune_visualize(void *ptr)
{
    return (struct kitsune_pointer*) (void*) (((u8*) ptr) - sizeof(usize));
}
