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

#include <kitsune/numbers.h>
#include <kitsune/iterator.h>
#include <kitsune/dynamic_iterator.h>
#include <kitsune/allocator.h>
#include <kitsune/alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <kitsune/list.h>
#include <assert.h>

void
iterate(struct kitsune_dynamic_iterator *iter)
{
        int *current = kitsune_iterator_next(&iter->base);
        while (current != NULL) {
                printf("%d\n", *current);
                current = kitsune_iterator_next(&iter->base);
        }
}

int
main()
{
        struct kitsune_allocator *a = kitsune_hardened_allocator;

        struct kitsune_list list = kitsune_list_init(sizeof(int), a);
        int val = 7;
        kitsune_list_push_back(&list, &val);
        val = 4;
        kitsune_list_push_back(&list, &val);
        val = 1;
        kitsune_list_push_back(&list, &val);
        val = 15;
        kitsune_list_push_back(&list, &val);

        struct kitsune_dynamic_iterator iter = kitsune_list_iterator(&list,
            false);

        iterate(&iter);
        val = 17;
        kitsune_list_push_back(&list, &val);
        printf("Added 17\n");
        int *current = kitsune_iterator_next(&iter.base);
        if (current != NULL)
                printf("%d\n", *current);

        current = kitsune_iterator_previous(&iter.base);
        if (current != NULL)
                printf("%d\n", *current);

        kitsune_dynamic_iterator_deinit(&iter);
        kitsune_list_deinit(&list, NULL);
        assert(kitsune_list_empty(&list) == true);
        return 0;
}
