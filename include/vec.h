#ifndef _VEC_MANAGED_H_
#define _VEC_MANAGED_H_

#include <allocator.h>
#include <numbers.h>

struct kitsune_vec {
    void *items;
    usize chunksize;
    usize size;
    struct kitsune_allocator *allocator;
};

typedef void kitsune_vec_deletor(struct kitsune_allocator*, void *);

struct kitsune_vec  kitsune_vec_init(usize, struct kitsune_allocator*);
void                kitsune_vec_deinit(struct kitsune_vec*,
                        kitsune_vec_deletor);
void                kitsune_vec_push(struct kitsune_vec*, void*);
void                kitsune_vec_insert(struct kitsune_vec*, usize, void*);
/*
 * BOTH POP and REMOVE ALLOCATE memory for the removed element!
 * Don't forget to free it!
 */
void               *kitsune_vec_pop(struct kitsune_vec*);
void               *kitsune_vec_remove(struct kitsune_vec*, usize);
usize               kitsune_vec_size(struct kitsune_vec*);
bool                kitsune_vec_empty(struct kitsune_vec*);

/* 
 * Iterator API
 * Will change once proper Iterator API is introduced
 */

void               *kitsune_vec_at(struct kitsune_vec*, usize);
void               *kitsune_vec_begin(struct kitsune_vec*);
void               *kitsune_vec_end(struct kitsune_vec*);
void               *kitsune_vec_rbegin(struct kitsune_vec*);
void               *kitsune_vec_rend(struct kitsune_vec*);


#endif
