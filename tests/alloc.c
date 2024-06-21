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
#include <assert.h>

int
main()
{
        struct kitsune_allocator *const a = kitsune_basic_allocator;
        int *arr = a->alloc(a, sizeof(int) * 2);

        arr[0] = 4;
        arr[1] = 3;

        assert(kitsune_allocated(arr) == sizeof(int) * 2);
        struct kitsune_pointer *ptr = kitsune_visualize(arr);
        assert(ptr->size == sizeof(int) * 2);

        a->free(a, arr);

        return 0;
}
