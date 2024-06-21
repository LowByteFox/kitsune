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
#include <kitsune/stack.h>
#include <assert.h>

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_stack stack = kitsune_stack_init(sizeof(int), a);
        int i;
        for (i = 0; i < 10; i++) {
                kitsune_stack_push(&stack, &i);
        }
        assert(kitsune_stack_size(&stack) == 10);
        assert(*(int*) kitsune_stack_top(&stack) == 9);

        for (i = 0; i < 10; i++) {
                int *val = kitsune_stack_pop(&stack);
                printf("%d\n", *val);
                a->free(a, val);
        }

        kitsune_stack_deinit(&stack, NULL);
        assert(kitsune_stack_empty(&stack) == true);
        return 0;
}
