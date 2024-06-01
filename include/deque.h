#ifndef _DEQUE_H_
#define _DEQUE_H_

#include <dynamic_iterator.h>
#include <vec.h>
#include <allocator.h>
#include <numbers.h>

struct kitsune_deque {
        struct kitsune_allocator *allocator;
        struct kitsune_vec front;
        struct kitsune_vec back;
        usize size;
};

struct kitsune_deque    kitsune_deque_init(usize, struct kitsune_allocator*);
void                    kitsune_deque_deinit(struct kitsune_deque*,
                            kitsune_allocator_deletor*);
void                    kitsune_deque_push_back(struct kitsune_deque*, void*);
void                    kitsune_deque_push_front(struct kitsune_deque*, void*);
void                    kitsune_deque_insert(struct kitsune_deque*, usize,
                            void*);
void                   *kitsune_deque_back(struct kitsune_deque*);
void                   *kitsune_deque_front(struct kitsune_deque*);
/*
 * BOTH POP AND REMOVE return ALLCATED memory by the deque
 * Don't forget to free it!
 */
void                   *kitsune_deque_pop_back(struct kitsune_deque*);
void                   *kitsune_deque_pop_front(struct kitsune_deque*);
void                   *kitsune_deque_remove(struct kitsune_deque*, usize);
void                   *kitsune_deque_get(struct kitsune_deque*, usize);
usize                   kitsune_deque_size(struct kitsune_deque*);
bool                    kitsune_deque_empty(struct kitsune_deque*);

/* Iterator API */
struct kitsune_dynamic_iterator kitsune_deque_iterator(struct kitsune_deque*);
struct kitsune_dynamic_iterator kitsune_deque_reverse_iterator(
                                    struct kitsune_deque*);

#endif
