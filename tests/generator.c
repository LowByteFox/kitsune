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

#include <kitsune/alloc/basic.h>
#include <kitsune/alloc/traced.h>
#include <kitsune/numbers.h>
#include <kitsune/generator.h>
#include <kitsune/iterator.h>
#include <kitsune/allocator.h>
#include <kitsune/alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <assert.h>

struct generator_data {
        struct kitsune_allocator *a;
        int number;
};

void*
generate(struct kitsune_generator *iter)
{
        struct generator_data *d = iter->context;
        d->number++;
        return &d->number;
}

void
deletor(struct kitsune_generator *iter)
{
        struct generator_data *d = iter->context;
        d->a->free(d->a, iter->context);
}

int
#ifdef ENABLE_RT
async_main()
#else
main()
#endif
{
        struct kitsune_allocator *allocator = kitsune_basic_allocator;

        struct kitsune_generator iter = kitsune_generator_init(
            sizeof(int), true, allocator, generate, deletor);

        struct generator_data *d = allocator->alloc(
            allocator, sizeof(struct generator_data));
        d->a = allocator;
        /* Just so first number is 0 */
        d->number = -1;
        kitsune_generator_set_context(&iter, d);

        while (true) {
                int *val = kitsune_iterator_next(&iter.base);
                printf("%d\n", *val);

                if (*val == 25)
                        break;
        }
        printf("Now we go back!\n");

        int *prev = kitsune_iterator_previous(&iter.base);
        while (prev != NULL) {
            printf("%d\n", *prev);
            prev = kitsune_iterator_previous(&iter.base);
        }

        kitsune_generator_deinit(&iter, NULL);
        assert(iter.result_cache.size == 0);
        return 0;
}
