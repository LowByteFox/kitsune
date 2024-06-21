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

#include <kitsune/alloc/hardened.h>
#include <kitsune/allocator.h>
#include <kitsune/alloc/traced.h>

int
main()
{
        struct kitsune_allocator *a = kitsune_hardened_allocator;
        struct kitsune_traced_allocator gpa = kitsune_traced_allocator_init(a);

        struct kitsune_allocator *allocator = 
            kitsune_traced_allocator_allocator(&gpa);

        int *hehe = allocator->alloc(allocator, sizeof(int) * 2);
        hehe[0] = 4;
        hehe[1] = 3;

        /* Trigger system allocator to move */
        hehe = allocator->resize(allocator, hehe, sizeof(int) * 100000); 

        /*
         * Lets leave this out :P
         * allocator->free(allocator, hehe);
         */

        kitsune_traced_allocator_deinit(&gpa);
        return 0;
}
