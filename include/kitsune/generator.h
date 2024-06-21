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

#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <kitsune/allocator.h>
#include <kitsune/numbers.h>
#include <kitsune/iterator.h>
#include <kitsune/vec.h>

/*
 * Generator
 * Each time the user calls next, new result will be generated
 * You can cache the result, if you don't - previous won't run
 */
struct kitsune_generator {
        struct kitsune_iterator base;
        struct kitsune_vec result_cache;

        void *context;
        void (*deletor)(struct kitsune_generator*);
        void *(*next)(struct kitsune_generator*);
        usize pos;
        bool cache_the_result;
};

typedef void *kitsune_generator_next(struct kitsune_generator*);

typedef void kitsune_generator_deletor(
    struct kitsune_generator*);

struct kitsune_generator kitsune_generator_init(usize, bool,
                                    struct kitsune_allocator*,
                                    kitsune_generator_next*,
                                    kitsune_generator_deletor*);
void                            kitsune_generator_set_context(
                                    struct kitsune_generator*, void*);
/*
 * Jumps to the end of the cache, so when you run next(), it will continue
 * instead of returning cached result
 */
void                            kitsune_generator_continue(
                                    struct kitsune_generator*);
void                            kitsune_generator_deinit(
                                    struct kitsune_generator*,
                                    kitsune_allocator_deletor*);

#endif
