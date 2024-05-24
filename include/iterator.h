#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include <numbers.h>

/* Used to tell the direction of memory for static iterator */
enum kitsune_iterator_direction {
        ADDITION,
        SUBSTRACTION
};

/* 
 * Simple static iterator
 * Results don't need to be cached as everything is known and already in the 
 * memory
 *
 * Doesn't need to be deinitilized
 */
struct kitsune_iterator {
        void *begin;
        void *current;
        usize chunk;
        void *end;
        enum kitsune_iterator_direction direction;
        bool is_dynamic;
};

/* Iterator API, dynamic iterator compatible */
struct kitsune_iterator kitsune_iterator_init(void*, void*, usize);
void                    kitsune_iterator_change_direction(
                            struct kitsune_iterator*,
                            enum kitsune_iterator_direction);
void                   *kitsune_iterator_next(struct kitsune_iterator*);
void                   *kitsune_iterator_previous(struct kitsune_iterator*);

#endif
