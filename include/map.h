#ifndef _MAP_H_
#define _MAP_H_

#include <dynamic_iterator.h>
#include <list.h>
#include <numbers.h>
#include <allocator.h>

struct kitsune_map_entry {
        void *key;
        usize keylen;
        void *value;
};

struct kitsune_map {
        struct kitsune_allocator *allocator;
        struct kitsune_list *items;
        usize datasize;
        usize size;
};

typedef void kitsune_map_deletor(struct kitsune_allocator*, void*);

struct kitsune_map  kitsune_map_init(usize, struct kitsune_allocator*);
void                kitsune_map_deinit(struct kitsune_map*,
                        kitsune_map_deletor*);
void                kitsune_map_insert(struct kitsune_map*, void*, usize,
                        void*);
void               *kitsune_map_remove(struct kitsune_map*, void*, usize);
void               *kitsune_map_at(struct kitsune_map*, void*, usize);
bool                kitsune_map_contains(struct kitsune_map*, void*, usize);
usize               kitsune_map_size(struct kitsune_map*);
usize               kitsune_map_capacity(struct kitsune_map*);
void                kitsune_map_reserve(struct kitsune_map*, usize);
bool                kitsune_map_empty(struct kitsune_map*);

/* Iterator API */
struct kitsune_dynamic_iterator kitsune_map_iterator(struct kitsune_map*);

#endif
