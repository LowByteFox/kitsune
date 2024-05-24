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
                /* XXX: Implement dynamic iterator */
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
                /* XXX: Implement dynamic iterator */
        }

        if (iter->current == iter->end)
                return NULL;

        if (iter->direction == ADDITION)
                iter->current = ((u8*) iter->current) - iter->chunk;
        else
                iter->current = ((u8*) iter->current) + iter->chunk;

        return iter->current;
}
