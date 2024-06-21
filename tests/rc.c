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
#include <kitsune/rc.h>
#include <assert.h>

void
deletor(struct kitsune_allocator *a, void *data)
{
        printf("%d is being deleted!\n", *(int*) data);
        a->free(a, data);
}

struct kitsune_rc*
givemerc(struct kitsune_allocator *a)
{
        int *data = a->alloc(a, sizeof(int));
        *data = 77;
        struct kitsune_rc *rc = kitsune_rc_init(data, a);

        return rc;
}

struct kitsune_rc*
test(struct kitsune_allocator *a)
{
        struct kitsune_rc* rc = givemerc(a);
        struct kitsune_rc* clone = kitsune_rc_clone(rc);
        assert(rc->count == 2);

        kitsune_rc_deinit(rc);

        assert(rc->count == 1);

        kitsune_rc_set_deletor(clone, deletor);

        printf("%d\n", *(int*) rc->data);

        return clone;
}

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        kitsune_rc_deinit(test(a));
        /* 
         * Memory should not leak
         * Also the rc shall not be used as it will deinit itself!
         */
        return 0;
}
