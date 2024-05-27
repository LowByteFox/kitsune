#ifndef _LIST_H_
#define _LIST_H_

#include <dynamic_iterator.h>
#include <allocator.h>
#include <numbers.h>

struct kitsune_list_node {
        struct kitsune_list_node *next;
        struct kitsune_list_node *previous;
        void *data;
};

struct kitsune_list {
    struct kitsune_allocator *allocator;
    usize datasize;
    struct kitsune_list_node *head;
    struct kitsune_list_node *tail;
    usize size;
};

typedef void kitsune_list_deletor(struct kitsune_allocator*, void*);

struct kitsune_list kitsune_list_init(usize, struct kitsune_allocator*);
void                kitsune_list_deinit(struct kitsune_list*,
                        kitsune_list_deletor*);
void                kitsune_list_append(struct kitsune_list*,
                        struct kitsune_list*);
void                kitsune_list_push_back(struct kitsune_list*, void*);
void                kitsune_list_push_front(struct kitsune_list*, void*);
void                kitsune_list_insert(struct kitsune_list*, usize, void*);
/*
 * BOTH POP AND REMOVE return ALLCATED memory by the linked list!
 * Don't forget to free it!
 */
void               *kitsune_list_pop_back(struct kitsune_list*);
void               *kitsune_list_pop_front(struct kitsune_list*);
void               *kitsune_list_remove(struct kitsune_list*, usize);
void               *kitsune_list_at(struct kitsune_list*, usize);
bool                kitsune_list_contains(struct kitsune_list*, void*);
usize               kitsune_list_size(struct kitsune_list*);
void                kitsune_list_resize(struct kitsune_list*, usize, void*,
                        kitsune_list_deletor*);
bool                kitsune_list_empty(struct kitsune_list*);

/* Iterator API */
struct kitsune_dynamic_iterator kitsune_list_iterator(struct kitsune_list*,
                                    bool);
struct kitsune_dynamic_iterator kitsune_list_reverse_iterator(
                                    struct kitsune_list*, bool);

void                            kitsune_list_iterator_set_circular(
                                    struct kitsune_dynamic_iterator*, bool);

#endif
