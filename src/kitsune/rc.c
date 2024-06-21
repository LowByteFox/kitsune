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
#include <kitsune/rc.h>

struct kitsune_rc*
kitsune_rc_init(void *data, struct kitsune_allocator *allocator)
{
        struct kitsune_rc *rc = allocator->alloc(allocator,
            sizeof(struct kitsune_rc));

        rc->allocator = allocator;
        rc->data = data;
        rc->count = 1;
        rc->deletor = NULL;

        return rc;
}

void
kitsune_rc_deinit(struct kitsune_rc *rc)
{
        rc->count--;

        if (rc->count == 0) {
                rc->deletor != NULL ? rc->deletor(rc->allocator, rc->data) :
                    rc->allocator->free(rc->allocator, rc->data);
                rc->allocator->free(rc->allocator, rc);
        }
}

void
kitsune_rc_set_deletor(struct kitsune_rc *rc, kitsune_allocator_deletor *deletor)
{
        rc->deletor = deletor;
}

struct kitsune_rc*
kitsune_rc_clone(struct kitsune_rc *rc)
{
        rc->count++;
        return rc;
}
