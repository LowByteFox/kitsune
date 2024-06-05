#ifndef _MAP_H_
#define _MAP_H_

#include <kitsune/dynamic_iterator.h>
#include <kitsune/vec.h>
#include <kitsune/numbers.h>
#include <kitsune/allocator.h>

struct kitsune_map_entry {
        void *key;
        usize keylen;
        void *value;
};

struct kitsune_map {
        struct kitsune_allocator *allocator;
        struct kitsune_vec *items;
        u64 (*hash_fn)(const void*, usize);
        usize datasize;
        usize size;
};

typedef u64         kitsune_map_hash(const void*, usize);

struct kitsune_map  kitsune_map_init(usize, struct kitsune_allocator*,
                        kitsune_map_hash*);
void                kitsune_map_deinit(struct kitsune_map*,
                        kitsune_allocator_deletor*);
void                kitsune_map_insert(struct kitsune_map*, void*, usize,
                        void*);
void               *kitsune_map_remove(struct kitsune_map*, void*, usize);
void               *kitsune_map_get(struct kitsune_map*, void*, usize);
bool                kitsune_map_contains(struct kitsune_map*, void*, usize);
usize               kitsune_map_size(struct kitsune_map*);
bool                kitsune_map_empty(struct kitsune_map*);

/* Iterator API */
struct kitsune_dynamic_iterator kitsune_map_iterator(struct kitsune_map*);
struct kitsune_dynamic_iterator kitsune_map_reverse_iterator(
                                    struct kitsune_map*);

#endif
