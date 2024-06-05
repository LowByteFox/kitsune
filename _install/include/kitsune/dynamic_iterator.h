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
