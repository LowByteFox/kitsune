/*
 * Copyright (C) 2024 LowByteFox
 * This file is part of kitsune.
 *
 * kitsune is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3.0 of the License, or
 * (at your option) any later version.
 *
 * kitsune is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with kitsune; if not, see <http://www.gnu.org/licenses/>.
 */

#include <kitsune/iterator.h>
#include <assert.h>
#include <kitsune/map.h>
#include <kitsune/memutils.h>
#include <kitsune/dynamic_iterator.h>
#include <kitsune/vec.h>
#include <kitsune/numbers.h>
#include <kitsune/allocator.h>

#define MAP_CHUNK 16
#define MAP_LOAD_FACTOR 0.9

static usize    kitsune_map_capacity(struct kitsune_map*);
static usize    kitsune_map_total_size(struct kitsune_map*);
static void     kitsune_map_resize(struct kitsune_map*, usize);
static void    *kitsune_map_iterator_next(struct kitsune_dynamic_iterator*);
static void    *kitsune_map_iterator_previous(
                    struct kitsune_dynamic_iterator*);
static void     kitsune_map_iterator_deletor(struct kitsune_dynamic_iterator*);

struct map_iter_ctx {
    struct kitsune_allocator *allocator;
    struct kitsune_map *map;
    usize pos;
    usize array_pos;
    struct kitsune_vec *current;
};

struct kitsune_map
kitsune_map_init(usize datasize, struct kitsune_allocator *allocator,
    kitsune_map_hash *hash_fn)
{
        struct kitsune_map map = {0};
        map.allocator = allocator;
        map.datasize = datasize;
        map.hash_fn = hash_fn;
        return map;
}

void
kitsune_map_deinit(struct kitsune_map *map, kitsune_allocator_deletor *deletor)
{
        usize i = 0;
        usize capacity = kitsune_map_capacity(map);
        for (; i < capacity; i++) {
                struct kitsune_vec *current = map->items + i;
                if (current->allocator == NULL) continue;

                struct kitsune_iterator iter = kitsune_vec_iterator(current);
                struct kitsune_map_entry *e = kitsune_iterator_next(&iter);

                for (; e != NULL; e = kitsune_iterator_next(&iter)) {
                        map->allocator->free(map->allocator, e->key);

                        deletor != NULL ? deletor(map->allocator, e->value) :
                            map->allocator->free(map->allocator, e->value);
                }

                kitsune_vec_deinit(current, NULL);
        }

        map->allocator->free(map->allocator, map->items);
        map->items = NULL;
        map->size = 0;
}

void
kitsune_map_insert(struct kitsune_map *map, void *key, usize keylen,
    void *data)
{
        bool first_init = false;
        if (map->size == 0) {
                kitsune_map_resize(map, MAP_CHUNK);
                first_init = true;
        }

        if (!first_init && (f64) kitsune_map_total_size(map) / 
            (f64) (kitsune_map_capacity(map) * MAP_CHUNK ) > MAP_LOAD_FACTOR)
                kitsune_map_resize(map, map->size * 2);

        struct kitsune_map_entry entry = {0};
        entry.key = kitsune_memdup(key, keylen, map->allocator);
        entry.keylen = keylen;
        entry.value = kitsune_memdup(data, map->datasize, map->allocator);

        u64 hash = map->hash_fn(key, keylen);
        usize index = hash % kitsune_map_capacity(map);
        struct kitsune_vec *current = map->items + index;
        if (current->allocator == NULL)
                *current = kitsune_vec_init(sizeof(struct kitsune_map_entry),
                    map->allocator);

        /* Entry will get coppied */
        kitsune_vec_push(current, &entry);
        map->size++;
}

void*
kitsune_map_remove(struct kitsune_map *map, void *key, usize keylen)
{
        if (map->size == 0)
                return NULL;

        void *data = NULL;

        u64 hash = map->hash_fn(key, keylen);
        usize index = hash % kitsune_map_capacity(map);
        struct kitsune_vec *current = map->items + index;
        if (current->allocator == NULL)
                return NULL;

        struct kitsune_iterator iter = kitsune_vec_iterator(current);
        struct kitsune_map_entry *entry = kitsune_iterator_next(&iter);
        usize i = 0;

        for (; entry != NULL; i++) {
                if (entry->keylen == keylen &&
                        kitsune_memcmp2(entry->key, key, keylen) == 0) {
                        data = entry->value;
                        entry->keylen = 0;
                        map->allocator->free(map->allocator, entry->key);
                        void *rmed = kitsune_vec_remove(current, i);
                        map->allocator->free(map->allocator, rmed);
                        map->size--;
                        break;
                }

                entry = kitsune_iterator_next(&iter);
        }

        return data;
}

void*
kitsune_map_get(struct kitsune_map *map, void *key, usize keylen)
{
        if (map->size == 0)
                return NULL;

        void *data = NULL;

        u64 hash = map->hash_fn(key, keylen);
        usize index = hash % kitsune_map_capacity(map);
        struct kitsune_vec *current = map->items + index;
        if (current->allocator == NULL)
                return NULL;

        struct kitsune_iterator iter = kitsune_vec_iterator(current);
        struct kitsune_map_entry *entry = kitsune_iterator_next(&iter);
        while (entry != NULL) {
                if (entry->keylen == keylen &&
                        kitsune_memcmp2(entry->key, key, keylen) == 0) {
                        data = entry->value;
                        break;
                }

                entry = kitsune_iterator_next(&iter);
        }

        return data;
}

bool
kitsune_map_contains(struct kitsune_map *map, void *key, usize keylen)
{
        if (map->size == 0)
                return false;
        u64 hash = map->hash_fn(key, keylen);
        usize index = hash % kitsune_map_capacity(map);
        struct kitsune_vec *current = map->items + index;
        if (current->allocator == NULL)
                return false;

        struct kitsune_iterator iter = kitsune_vec_iterator(current);
        struct kitsune_map_entry *entry = kitsune_iterator_next(&iter);

        while (entry != NULL) {
                if (entry->keylen == keylen &&
                        kitsune_memcmp2(entry->key, key, keylen) == 0) {
                        return true;
                }

                entry = kitsune_iterator_next(&iter);
        }

        return false;
}

usize
kitsune_map_size(struct kitsune_map *map)
{
        return map->size;
}

bool
kitsune_map_empty(struct kitsune_map *map)
{
        return map->size == 0;
}

static usize
kitsune_map_capacity(struct kitsune_map *map)
{
        return kitsune_allocated(map->items) / sizeof(struct kitsune_vec);
}

static usize
kitsune_map_total_size(struct kitsune_map *map)
{
        usize total = 0;
        usize i = 0;
        for (; i < map->size; i++) {
            total += map->items[i].size;
        }

        return total;
}

struct kitsune_dynamic_iterator
kitsune_map_iterator(struct kitsune_map *map)
{
        struct kitsune_dynamic_iterator iter = kitsune_dynamic_iterator_init(
            kitsune_map_iterator_next,
            kitsune_map_iterator_previous,
            kitsune_map_iterator_deletor);

        struct map_iter_ctx *ctx = map->allocator->alloc(map->allocator,
            sizeof(struct map_iter_ctx));
        ctx->allocator = map->allocator;
        ctx->map = map;
        ctx->current = NULL;
        ctx->pos = 0;
        ctx->array_pos = 0;

        kitsune_dynamic_iterator_set_context(&iter, ctx);

        return iter;
}

struct kitsune_dynamic_iterator
kitsune_map_reverse_iterator(struct kitsune_map *map)
{
        struct kitsune_dynamic_iterator iter = kitsune_dynamic_iterator_init(
            kitsune_map_iterator_next,
            kitsune_map_iterator_previous,
            kitsune_map_iterator_deletor);
        iter.base.direction = SUBSTRACTION;

        struct map_iter_ctx *ctx = map->allocator->alloc(map->allocator,
            sizeof(struct map_iter_ctx));
        ctx->allocator = map->allocator;
        ctx->map = map;
        ctx->current = NULL;
        ctx->pos = kitsune_map_capacity(map) - 1;
        ctx->array_pos = 0;

        kitsune_dynamic_iterator_set_context(&iter, ctx);

        return iter;
}

static void
kitsune_map_resize(struct kitsune_map *map, usize new_capacity)
{
        struct kitsune_vec *new_items = map->allocator->alloc(map->allocator,
            sizeof(struct kitsune_vec) * new_capacity);
        kitsune_memset2(new_items, 0, kitsune_allocated(new_items));

        usize i = 0;
        usize capacity = kitsune_map_capacity(map);
        for (; i < capacity; i++) {
                struct kitsune_vec *current = map->items + i;
                if (current->allocator == NULL) continue;

                struct kitsune_iterator iter = kitsune_vec_iterator(current);

                struct kitsune_map_entry *entry = kitsune_iterator_next(&iter);
                
                while (entry != NULL) {
                        u64 hash = map->hash_fn(entry->key, entry->keylen);
                        usize index = hash % new_capacity;
                        struct kitsune_vec *new_current = new_items + index;
                        if (new_current->allocator == NULL)
                                *new_current = kitsune_vec_init(
                                    sizeof(struct kitsune_map_entry),
                                    map->allocator);

                        kitsune_vec_push(new_current, entry);
                        entry = kitsune_iterator_next(&iter);
                }

                kitsune_vec_deinit(current, NULL);
        }
        map->allocator->free(map->allocator, map->items);

        map->items = new_items;
}

static void*
kitsune_map_iterator_next(struct kitsune_dynamic_iterator *iter)
{
        struct map_iter_ctx *ctx = iter->context;
start:
        if (ctx->current == NULL) {
                usize capacity = kitsune_map_capacity(ctx->map);
                usize direction = iter->base.direction == ADDITION ? 1 : -1;

                for (; ctx->pos < capacity && ctx->pos >= 0;
                    ctx->pos += direction) {
                        struct kitsune_vec *current = ctx->map->items +
                            ctx->pos;
                        if (current->allocator == NULL) continue;

                        ctx->current = current;
                        break;
                }

                if (ctx->current != NULL &&
                    iter->base.direction == SUBSTRACTION)
                        ctx->array_pos = ctx->current->size - 1;
        }

        if (ctx->current == NULL)
                return NULL;

        struct kitsune_map_entry *item = kitsune_vec_get(ctx->current,
            ctx->array_pos);

        ctx->array_pos += iter->base.direction == ADDITION ? 1 : -1;

        if (item == NULL) {
                ctx->current = NULL;
                ctx->array_pos = 0;
                ctx->pos += iter->base.direction == ADDITION ? 1 : -1;
                goto start;
        }

        return item;
}

static void*
kitsune_map_iterator_previous(struct kitsune_dynamic_iterator *iter)
{
        enum kitsune_iterator_direction dir = iter->base.direction;
        enum kitsune_iterator_direction swap = ADDITION;

        switch (dir) {
        case ADDITION:
                swap = SUBSTRACTION;
                break;
        case SUBSTRACTION:
                swap = ADDITION;
                break;
        }

        iter->base.direction = swap;

        void *item = kitsune_map_iterator_next(iter);
        iter->base.direction = dir;

        return item;
}

static void
kitsune_map_iterator_deletor(struct kitsune_dynamic_iterator *iter)
{
        struct map_iter_ctx *ctx = iter->context;
        ctx->allocator->free(ctx->allocator, iter->context);
}
