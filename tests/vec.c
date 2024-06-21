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
#include <stdio.h>
#include <strings.h>
#include <kitsune/vec.h>
#include <assert.h>

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_vec vec = kitsune_vec_init(sizeof(int), a);
        int val = 7;

        kitsune_vec_push(&vec, &val);
        val = 77;
        kitsune_vec_push(&vec, &val);
        val = 777;
        kitsune_vec_insert(&vec, 7, &val);
        kitsune_vec_insert(&vec, 2, &val);
        kitsune_vec_insert(&vec, 2, &val);
        val = 77;
        kitsune_vec_push(&vec, &val);
        val = 7;
        kitsune_vec_insert(&vec, 7, &val);
        assert(kitsune_vec_size(&vec) == 7);

        int *removed = kitsune_vec_remove(&vec, vec.size - 2);
        assert(*removed == 77);
        a->free(a, removed);
        removed = kitsune_vec_pop(&vec);
        assert(*removed == 7);
        a->free(a, removed);

        assert(kitsune_vec_size(&vec) == 5);

        kitsune_vec_deinit(&vec, NULL);

        assert(kitsune_vec_empty(&vec) == true);
        return 0;
}
