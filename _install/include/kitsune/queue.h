#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <kitsune/numbers.h>
#include <kitsune/list.h>
#include <kitsune/allocator.h>

struct kitsune_queue {
        struct kitsune_list list;
};

struct kitsune_queue    kitsune_queue_init(usize, struct kitsune_allocator*);
void                    kitsune_queue_deinit(struct kitsune_queue*,
                            kitsune_allocator_deletor*);
void                    kitsune_queue_push(struct kitsune_queue*, void*);
/*
 * POP ALLOCATES memory for the removed element
 * Don't forget to remove it
 */
void                   *kitsune_queue_pop(struct kitsune_queue*);
void                   *kitsune_queue_front(struct kitsune_queue*);
void                   *kitsune_queue_back(struct kitsune_queue*);
usize                   kitsune_queue_size(struct kitsune_queue*);
bool                    kitsune_queue_empty(struct kitsune_queue*);

#endif
