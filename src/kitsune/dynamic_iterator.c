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

struct kitsune_dynamic_iterator
kitsune_dynamic_iterator_init(kitsune_dynamic_iterator_fn *next,
    kitsune_dynamic_iterator_fn *previous,
    kitsune_dynamic_iterator_deletor *deletor)
{
        struct kitsune_dynamic_iterator iter = {0};
        iter.base = kitsune_iterator_init(NULL, NULL, 0);
        iter.base.kind = DYNAMIC;
        iter.next = next;
        iter.previous = previous;
        iter.deletor = deletor;

        return iter;
}

void
kitsune_dynamic_iterator_set_context(struct kitsune_dynamic_iterator *iter,
    void *context)
{
        iter->context = context;
}

void
kitsune_dynamic_iterator_deinit(struct kitsune_dynamic_iterator *iter)
{
        iter->deletor != NULL ? iter->deletor(iter) : NULL;
}
