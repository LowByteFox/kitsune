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

#include <kitsune/iterator.h>
#include <kitsune/allocator.h>
#include <kitsune/alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <kitsune/vec.h>
#include <assert.h>

void
iterate(struct kitsune_iterator *i)
{
        int *current = kitsune_iterator_next(i);
        while (current != NULL) {
                printf("%d\n", *current);
                current = kitsune_iterator_next(i);
        }
}

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_vec vec = kitsune_vec_init(sizeof(int), a);
        int val = 7;
        kitsune_vec_push(&vec, &val);
        val = 4;
        kitsune_vec_push(&vec, &val);
        val = 1;
        kitsune_vec_push(&vec, &val);
        val = 15;
        kitsune_vec_push(&vec, &val);

        struct kitsune_iterator iter = kitsune_vec_iterator(&vec);
        iterate(&iter);
        printf("Now reverse!\n");
        iter = kitsune_vec_reverse_iterator(&vec);
        iterate(&iter);

        kitsune_vec_deinit(&vec, NULL);
        assert(kitsune_vec_empty(&vec) == true);
        return 0;
}
