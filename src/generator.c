#include <allocator.h>
#include <vec.h>
#include <numbers.h>
#include <iterator.h>
#include <generator.h>

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
