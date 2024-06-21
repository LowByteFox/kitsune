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
#include <kitsune/dynamic_iterator.h>
#include <kitsune/numbers.h>
#include <kitsune/allocator.h>
#include <kitsune/alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <kitsune/deque.h>
#include <assert.h>

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_deque deq = kitsune_deque_init(sizeof(int), a);

        int val = 10;
        kitsune_deque_push_front(&deq, &val);
        val = 20;
        kitsune_deque_push_front(&deq, &val);
        val = 30;
        kitsune_deque_push_back(&deq, &val);
        val = 40;
        kitsune_deque_push_back(&deq, &val);
        val = 5;
        kitsune_deque_insert(&deq, 3, &val); /* 20, 10, 30, 5, 40 */

        struct kitsune_dynamic_iterator iter = kitsune_deque_iterator(&deq);

        int *item = kitsune_iterator_next(&iter.base);
        while (item != NULL) {
                printf("%d ", *item);
                item = kitsune_iterator_next(&iter.base);
        }
        putchar(10);

        kitsune_dynamic_iterator_deinit(&iter);
        kitsune_deque_deinit(&deq, NULL);

        assert(kitsune_deque_empty(&deq) == true);
        return 0;
}
