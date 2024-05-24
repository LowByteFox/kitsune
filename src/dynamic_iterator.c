#include "vec.h"
#include <numbers.h>
#include <iterator.h>
#include <dynamic_iterator.h>

struct kitsune_dynamic_iterator
kitsune_dynamic_iterator_init(usize chunksize, bool cache_results,
    struct kitsune_allocator *allocator,
    kitsune_dynamic_iterator_next *next_fn,
    kitsune_dynamic_iterator_deletor *deletor)
{
        struct kitsune_dynamic_iterator iter = {0};
        iter.base = kitsune_iterator_init(NULL, NULL, 0);
        iter.base.is_dynamic = true;
        iter.cache_the_result = cache_results;
        if (cache_results)
                iter.result_cache = kitsune_vec_init(chunksize, allocator);
        iter.next = next_fn;
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
kitsune_dynamic_iterator_continue(struct kitsune_dynamic_iterator *iter)
{
        if (!iter->cache_the_result)
                return;
        iter->pos = iter->result_cache.size;
}

void
kitsune_dynamic_iterator_deinit(struct kitsune_dynamic_iterator *iter)
{
        if (iter->cache_the_result)
                kitsune_vec_deinit(&iter->result_cache, NULL);

        iter->deletor != NULL ? iter->deletor(iter) : NULL;
}
