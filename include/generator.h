#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <allocator.h>
#include <numbers.h>
#include <iterator.h>
#include <vec.h>

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
                                    kitsune_vec_deletor*);

#endif
