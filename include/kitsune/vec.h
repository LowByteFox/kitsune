#ifndef _VEC_H_
#define _VEC_H_

#include <kitsune/allocator.h>
#include <kitsune/iterator.h>
#include <kitsune/dynamic_iterator.h>
#include <kitsune/numbers.h>

struct kitsune_vec {
        struct kitsune_allocator *allocator;
        usize chunksize;
        void *items;
        usize size;
};

struct kitsune_vec   kitsune_vec_init(usize, struct kitsune_allocator*);
void                 kitsune_vec_deinit(struct kitsune_vec*,
                        kitsune_allocator_deletor*);
void                 kitsune_vec_push(struct kitsune_vec*, void*);
void                 kitsune_vec_insert(struct kitsune_vec*, usize, void*);
/*
 * BOTH POP and REMOVE ALLOCATE memory for the removed element!
 * Don't forget to free it!
 */
void                *kitsune_vec_pop(struct kitsune_vec*);
void                *kitsune_vec_front(struct kitsune_vec*);
void                *kitsune_vec_back(struct kitsune_vec*);
void                *kitsune_vec_remove(struct kitsune_vec*, usize);
void                *kitsune_vec_get(struct kitsune_vec*, usize);
bool                 kitsune_vec_contains(struct kitsune_vec*, void*);
usize                kitsune_vec_size(struct kitsune_vec*);
usize                kitsune_vec_capacity(struct kitsune_vec*);
void                 kitsune_vec_reserve(struct kitsune_vec*, usize);
void                 kitsune_vec_shrink_to_fit(struct kitsune_vec*);
bool                 kitsune_vec_empty(struct kitsune_vec*);

/* Iterator API */
struct kitsune_iterator         kitsune_vec_iterator(
                                    struct kitsune_vec*);
struct kitsune_iterator         kitsune_vec_reverse_iterator(
                                    struct kitsune_vec*);
struct kitsune_dynamic_iterator kitsune_vec_dynamic_iterator(
                                    struct kitsune_vec*);
struct kitsune_dynamic_iterator kitsune_vec_reverse_dynamic_iterator(
                                    struct kitsune_vec*);

#endif
