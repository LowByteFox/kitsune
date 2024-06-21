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

#ifndef _DYNAMIC_ITERATOR_H_
#define _DYNAMIC_ITERATOR_H_

#include <kitsune/numbers.h>
#include <kitsune/iterator.h>

/*
 * Dynamic iterator API
 * Results don't need to be cached as everything is known and already in the 
 * memory
 *
 * dynamic iterator is not sensitive to invalidation unlike static iterator,
 * however it is slower
 */
struct kitsune_dynamic_iterator { 
        struct kitsune_iterator base;

        void *context;
        void (*deletor)(struct kitsune_dynamic_iterator*);
        void *(*next)(struct kitsune_dynamic_iterator*);
        void *(*previous)(struct kitsune_dynamic_iterator*);
};

typedef void *kitsune_dynamic_iterator_fn(struct kitsune_dynamic_iterator*);

typedef void kitsune_dynamic_iterator_deletor(
    struct kitsune_dynamic_iterator*);

struct kitsune_dynamic_iterator kitsune_dynamic_iterator_init(
                                    kitsune_dynamic_iterator_fn*,
                                    kitsune_dynamic_iterator_fn*,
                                    kitsune_dynamic_iterator_deletor*);
void                            kitsune_dynamic_iterator_set_context(
                                    struct kitsune_dynamic_iterator*, void*);
void                            kitsune_dynamic_iterator_deinit(
                                    struct kitsune_dynamic_iterator*);

#endif
