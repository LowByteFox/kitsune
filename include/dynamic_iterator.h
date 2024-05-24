#ifndef _DYNAMIC_ITERATOR_H_
#define _DYNAMIC_ITERATOR_H_

#include <allocator.h>
#include <numbers.h>
#include <iterator.h>
#include <vec.h>

/*
 * Advanced dynamic iterator
 * Each time the user calls next, new result will be generated
 * You can cache the result, if you don't previous won't run
 */
struct kitsune_dynamic_iterator {
        struct kitsune_iterator base;
        struct kitsune_vec result_cache;

        void *context;
        void (*deletor)(struct kitsune_dynamic_iterator*);
        void *(*next)(struct kitsune_dynamic_iterator*);
        usize pos;
        bool cache_the_result;
};

typedef void *kitsune_dynamic_iterator_next(struct kitsune_dynamic_iterator*);

typedef void kitsune_dynamic_iterator_deletor(
    struct kitsune_dynamic_iterator*);

/* Dynamic Iterator API */
struct kitsune_dynamic_iterator kitsune_dynamic_iterator_init(usize, bool,
                                    struct kitsune_allocator*,
                                    kitsune_dynamic_iterator_next*,
                                    kitsune_dynamic_iterator_deletor*);
void                            kitsune_dynamic_iterator_set_context(
                                    struct kitsune_dynamic_iterator*, void*);
/*
 * Jumps to the end of the cache, so when you run next(), it will continue
 * instead of returning cached result
 */
void                            kitsune_dynamic_iterator_continue(
                                    struct kitsune_dynamic_iterator*);
void                            kitsune_dynamic_iterator_deinit(
                                    struct kitsune_dynamic_iterator*);

#endif
