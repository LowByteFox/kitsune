#include <vec.h>
#include <assert.h>
#include <dynamic_iterator.h>
#include <numbers.h>
#include <iterator.h>

struct kitsune_iterator
kitsune_iterator_init(void *begin, void *end, usize chunksize)
{
        struct kitsune_iterator iter = {0};
        iter.begin = begin;
        iter.current = begin;
        iter.end = end;
        iter.chunk = chunksize;
        iter.direction = ADDITION;
        iter.is_dynamic = false;

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
        if (iter->is_dynamic) {
                /* the iterator is at offset 0, so I can allow this */
                struct kitsune_dynamic_iterator *i = (void*) iter;

                if (!i->cache_the_result)
                        return i->next(i);

                assert(i->pos > 0);
                if (i->pos < i->result_cache.size) {
                        void *current = kitsune_vec_at(&i->result_cache,
                                            i->pos);
                        i->pos++;
                        return current;
                }

                void *result = i->next(i);
                kitsune_vec_push(&i->result_cache, result);
                return result;
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
        if (iter->is_dynamic) {
                /* the iterator is at offset 0, so I can allow this */
                struct kitsune_dynamic_iterator *i = (void*) iter;

                if (!i->cache_the_result)
                        return NULL;
                if (i->pos == 0)
                        return NULL;

                i->pos--;
                return kitsune_vec_at(&i->result_cache, i->pos);
        }

        if (iter->current == iter->end)
                return NULL;

        if (iter->direction == ADDITION)
                iter->current = ((u8*) iter->current) - iter->chunk;
        else
                iter->current = ((u8*) iter->current) + iter->chunk;

        return iter->current;
}
