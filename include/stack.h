#ifndef _STACK_H_
#define _STACK_H_

#include <allocator.h>
#include <numbers.h>

struct kitsune_stack {
        struct kitsune_allocator *allocator;
        usize chunksize;
        void *items;
        usize size;
};

struct kitsune_stack    kitsune_stack_init(usize, struct kitsune_allocator*);
void                    kitsune_stack_deinit(struct kitsune_stack*,
                            kitsune_allocator_deletor*);
void                    kitsune_stack_push(struct kitsune_stack*, void*);
/*
 * POP ALLOCATES memory for the removed element
 * Don't forget to free it!
 */
void                   *kitsune_stack_pop(struct kitsune_stack*);
void                   *kitsune_stack_top(struct kitsune_stack*);
usize                   kitsune_stack_size(struct kitsune_stack*);
bool                    kitsune_stack_empty(struct kitsune_stack*);

#endif
