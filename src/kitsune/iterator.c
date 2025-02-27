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
#include <kitsune/vec.h>
#include <assert.h>
#include <kitsune/generator.h>
#include <kitsune/numbers.h>
#include <kitsune/iterator.h>

struct kitsune_iterator
kitsune_iterator_init(void *begin, void *end, usize chunksize)
{
        struct kitsune_iterator iter = {0};
        iter.begin = begin;
        iter.current = begin;
        iter.end = end;
        iter.chunk = chunksize;
        iter.direction = ADDITION;
        iter.kind = STATIC;

        return iter;
}

void
kitsune_iterator_change_direction(struct kitsune_iterator *iter,
    enum kitsune_iterator_direction direction)
{
        iter->direction = direction;
}

void*
kitsune_iterator_next(struct kitsune_iterator *iter)
{
        switch (iter->kind) {
        case GENERATOR: {
                /* the iterator is at offset 0, so I can allow this */
                struct kitsune_generator *i = (void*) iter;

                if (!i->cache_the_result)
                        return i->next(i);

                if (i->pos < i->result_cache.size) {
                        void *current = kitsune_vec_get(&i->result_cache,
                                            i->pos);
                        i->pos++;
                        return current;
                }

                void *result = i->next(i);
                kitsune_vec_push(&i->result_cache, result);
                i->pos++;
                return result;
        }
        case DYNAMIC: {
                struct kitsune_dynamic_iterator *i = (void*) iter;
                return i->next(i);
        }
        default:
                break;
        }

        if (iter->current == iter->end)
                return NULL;

        void *current = iter->current;
        if (iter->direction == ADDITION)
                iter->current = ((u8*) iter->current) + iter->chunk;
        else
                iter->current = ((u8*) iter->current) - iter->chunk;

        return current;
}

void*
kitsune_iterator_previous(struct kitsune_iterator *iter)
{
        switch (iter->kind) {
        case GENERATOR: {
                /* the iterator is at offset 0, so I can allow this */
                struct kitsune_generator *i = (void*) iter;
                if (!i->cache_the_result)
                        return NULL;
                if (i->pos == 0)
                        return NULL;

                i->pos--;
                return kitsune_vec_get(&i->result_cache, i->pos);
        }
        case DYNAMIC: {
                struct kitsune_dynamic_iterator *i = (void*) iter;
                return i->previous(i);
        }
        default:
                break;
        }

        if (iter->current == iter->end)
                return NULL;

        if (iter->direction == ADDITION)
                iter->current = ((u8*) iter->current) - iter->chunk;
        else
                iter->current = ((u8*) iter->current) + iter->chunk;

        return iter->current;
}
