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
#include <kitsune/vec.h>
#include <kitsune/numbers.h>
#include <kitsune/iterator.h>
#include <kitsune/generator.h>

struct kitsune_generator
kitsune_generator_init(usize chunksize, bool cache_results,
    struct kitsune_allocator *allocator,
    kitsune_generator_next *next_fn,
    kitsune_generator_deletor *deletor)
{
        struct kitsune_generator generator = {0};
        generator.base = kitsune_iterator_init(NULL, NULL, 0);
        generator.base.kind = GENERATOR;
        generator.cache_the_result = cache_results;
        if (cache_results)
                generator.result_cache = kitsune_vec_init(chunksize, allocator);
        generator.next = next_fn;
        generator.deletor = deletor;

        return generator;
}

void
kitsune_generator_set_context(struct kitsune_generator *generator,
        void *context)
{
        generator->context = context;
}

void
kitsune_generator_continue(struct kitsune_generator *generator)
{
        if (!generator->cache_the_result)
                return;
        generator->pos = generator->result_cache.size;
}

void
kitsune_generator_deinit(struct kitsune_generator *generator,
    kitsune_allocator_deletor *deletor)
{
        if (generator->cache_the_result)
                kitsune_vec_deinit(&generator->result_cache, deletor);

        generator->deletor != NULL ? generator->deletor(generator) : NULL;
}
