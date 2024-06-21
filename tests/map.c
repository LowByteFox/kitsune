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

#include <kitsune/dynamic_iterator.h>
#include <kitsune/iterator.h>
#include <kitsune/crashtrace.h>
#include <kitsune/allocator.h>
#include <kitsune/alloc/basic.h>
#include <kitsune/alloc/traced.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <kitsune/map.h>
#include <kitsune/hashes.h>
#include <assert.h>

int
main()
{
        kitsune_install_crashtrace();
        struct kitsune_allocator *a = kitsune_basic_allocator;

        struct kitsune_traced_allocator gpa = kitsune_traced_allocator_init(a);

        struct kitsune_allocator *allocator = 
            kitsune_traced_allocator_allocator(&gpa);

        struct kitsune_map map = kitsune_map_init(sizeof(int), allocator,
            kitsune_fnv1a_64);

        int val = 7;
        kitsune_map_insert(&map, "Hello!", 7, &val);
        val = 50;
        kitsune_map_insert(&map, "Hey!", 5, &val);
        val = 10;
        kitsune_map_insert(&map, "uwu!", 5, &val);
        val = 4;
        kitsune_map_insert(&map, "owo", 4, &val);

        struct kitsune_dynamic_iterator iter =
            kitsune_map_reverse_iterator(&map);
        struct kitsune_map_entry *item = kitsune_iterator_next(&iter.base);

        printf("%s\t=> %d\n", (char*) item->key, *(int*) item->value);

        item = kitsune_iterator_next(&iter.base);
        if (item != NULL)
                printf("%s\t=> %d\n", (char*) item->key, *(int*) item->value);

        item = kitsune_iterator_previous(&iter.base);
        if (item != NULL)
                printf("%s\t=> %d\n", (char*) item->key, *(int*) item->value);

        kitsune_dynamic_iterator_deinit(&iter);
        kitsune_map_deinit(&map, NULL);
        assert(kitsune_map_empty(&map) == true);
        kitsune_traced_allocator_deinit(&gpa);

        printf("done!\n");
        return 0;
}
