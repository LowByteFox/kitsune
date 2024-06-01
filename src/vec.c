#include <dynamic_iterator.h>
#include <allocator.h>
#include <iterator.h>
#include <assert.h>
#include <numbers.h>
#include <stdio.h>
#include <vec.h>
#include <memutils.h>

#define VEC_CHUNK 16

static void    *kitsune_vec_begin(struct kitsune_vec*);
static void    *kitsune_vec_end(struct kitsune_vec*);
static void    *kitsune_vec_rbegin(struct kitsune_vec*);
static void    *kitsune_vec_rend(struct kitsune_vec*);
static void    *kitsune_vec_dynamic_iterator_next(
                    struct kitsune_dynamic_iterator*);
static void    *kitsune_vec_dynamic_iterator_previous(
                    struct kitsune_dynamic_iterator*);
static void     kitsune_vec_dynamic_iterator_deletor(
                    struct kitsune_dynamic_iterator*);

struct vec_iter_ctx {
        struct kitsune_allocator *allocator;
        struct kitsune_vec *vec;
        usize pos;
};

struct kitsune_vec
kitsune_vec_init(usize chunksize, struct kitsune_allocator *allocator)
{
        struct kitsune_vec vec = {0};
        vec.chunksize = chunksize;
        vec.allocator = allocator;
    
        return vec;
}

void
kitsune_vec_deinit(struct kitsune_vec *vec, kitsune_allocator_deletor *deletor)
{
        void *iter = kitsune_vec_begin(vec);
        void *end = kitsune_vec_end(vec);

        while (iter != end) {
                deletor != NULL ? deletor(vec->allocator, iter) : NULL;
                iter = ((u8*) iter) + vec->chunksize;
        }

        vec->allocator->free(vec->allocator, vec->items);
        vec->items = NULL;
        vec->size = 0;
}

void
kitsune_vec_push(struct kitsune_vec *vec, void *data)
{
        usize capacity = kitsune_vec_capacity(vec);
        if (vec->size == capacity)
                kitsune_vec_reserve(vec, capacity + VEC_CHUNK);
        vec->size++;

        u8 *ptr = vec->items;
        kitsune_memcpy(ptr + (vec->size - 1) * vec->chunksize, vec->chunksize, 
            data, vec->chunksize);
}

void
*kitsune_vec_pop(struct kitsune_vec *vec)
{
        if (vec->size == 0)
                return NULL;

        void *data = vec->allocator->alloc(vec->allocator, vec->chunksize);
        kitsune_memcpy(data, vec->chunksize, kitsune_vec_rbegin(vec),
            vec->chunksize);

        vec->size--;

        usize capacity = kitsune_vec_capacity(vec);
        if (vec->size + VEC_CHUNK == capacity - 1)
                kitsune_vec_shrink_to_fit(vec);
        return data;
}

void
kitsune_vec_insert(struct kitsune_vec *vec, usize index, void *data)
{
        if (index >= vec->size) {
                kitsune_vec_push(vec, data);
                return;
        }

        usize capacity = kitsune_vec_capacity(vec);
        if (vec->size == capacity)
                kitsune_vec_reserve(vec, capacity + VEC_CHUNK);
        vec->size++;

        u8 *ptr = vec->items;

        kitsune_rmemcpy(ptr + vec->chunksize * (index + 1),
            (vec->size - index) * vec->chunksize,
            ptr + vec->chunksize * index,
            (vec->size - 1 - index) * vec->chunksize);

        kitsune_memcpy(ptr + index * vec->chunksize, vec->chunksize, 
            data, vec->chunksize);
}

void*
kitsune_vec_remove(struct kitsune_vec *vec, usize index)
{
        if (index >= vec->size)
                return kitsune_vec_pop(vec);

        u8 *ptr = vec->items;

        void *data = vec->allocator->alloc(vec->allocator, vec->chunksize);
        kitsune_memcpy(data, vec->chunksize, ptr + index * vec->chunksize,
            vec->chunksize);

        kitsune_memcpy(ptr + vec->chunksize * index,
            (vec->size - index) * vec->chunksize,
            ptr + vec->chunksize * (index + 1),
            (vec->size - 1 - index) * vec->chunksize);

        vec->size--;

        usize capacity = kitsune_vec_capacity(vec);
        if (vec->size + VEC_CHUNK == capacity)
                kitsune_vec_shrink_to_fit(vec);

        return data;
}

bool
kitsune_vec_contains(struct kitsune_vec *vec, void *data)
{
        void *iter = kitsune_vec_begin(vec);
        void *end = kitsune_vec_end(vec);

        while (iter != end) {
                if (kitsune_memcmp(iter, vec->chunksize, data, vec->chunksize)
                    == 0)
                        return true;
                iter = ((u8*) iter) + vec->chunksize;
        }

        return false;
}

usize
kitsune_vec_capacity(struct kitsune_vec *vec)
{
        usize allocated = kitsune_allocated(vec->items);
        return allocated / vec->chunksize;
}

void
kitsune_vec_reserve(struct kitsune_vec *vec, usize new_capacity)
{
        usize capacity = kitsune_vec_capacity(vec);
        if (new_capacity <= capacity)
                return;

        vec->items = vec->allocator->resize(vec->allocator, vec->items,
                new_capacity * vec->chunksize);
}

void
kitsune_vec_shrink_to_fit(struct kitsune_vec *vec)
{
        usize allocated = kitsune_allocated(vec->items);
        if (allocated == vec->size * vec->chunksize)
                return;

        vec->items = vec->allocator->resize(vec->allocator, vec->items,
                vec->size * vec->chunksize);
}

usize
kitsune_vec_size(struct kitsune_vec *vec)
{
        return vec->size;
}

bool
kitsune_vec_empty(struct kitsune_vec *vec)
{
        return vec->size == 0;
}

void*
kitsune_vec_at(struct kitsune_vec *vec, usize index)
{
        if (vec->size <= index) return NULL;

        u8 *ptr = vec->items;
        return ptr + index * vec->chunksize;
}

static void*
kitsune_vec_begin(struct kitsune_vec *vec)
{
        return vec->items;
}

static void*
kitsune_vec_end(struct kitsune_vec *vec)
{
        return ((u8*) vec->items) + vec->chunksize * vec->size;
}

static void*
kitsune_vec_rbegin(struct kitsune_vec *vec)
{
        return vec->size == 0 ? kitsune_vec_end(vec) : ((u8*) vec->items) +
            vec->chunksize * (vec->size - 1);
}

static void*
kitsune_vec_rend(struct kitsune_vec *vec)
{
        return ((u8*) vec->items) - vec->chunksize;
}

struct kitsune_iterator
kitsune_vec_iterator(struct kitsune_vec *vec)
{
        struct kitsune_iterator iter = kitsune_iterator_init(
            kitsune_vec_begin(vec), kitsune_vec_end(vec), vec->chunksize);
        return iter;
}

struct kitsune_iterator
kitsune_vec_reverse_iterator(struct kitsune_vec *vec)
{
        struct kitsune_iterator iter = kitsune_iterator_init(
            kitsune_vec_rbegin(vec), kitsune_vec_rend(vec), vec->chunksize);
        kitsune_iterator_change_direction(&iter, SUBSTRACTION);
        return iter;
}

struct kitsune_dynamic_iterator
kitsune_vec_dynamic_iterator(struct kitsune_vec *vec)
{
        struct vec_iter_ctx *ctx = vec->allocator->alloc(vec->allocator,
            sizeof(struct vec_iter_ctx));
        ctx->allocator = vec->allocator;
        ctx->vec = vec;
        ctx->pos = 0;

        struct kitsune_dynamic_iterator iter = kitsune_dynamic_iterator_init(
            kitsune_vec_dynamic_iterator_next,
            kitsune_vec_dynamic_iterator_previous,
            kitsune_vec_dynamic_iterator_deletor);
        kitsune_dynamic_iterator_set_context(&iter, ctx);

        return iter;
}

struct kitsune_dynamic_iterator
kitsune_vec_reverse_dynamic_iterator(struct kitsune_vec *vec)
{
        struct vec_iter_ctx *ctx = vec->allocator->alloc(vec->allocator,
            sizeof(struct vec_iter_ctx));
        ctx->allocator = vec->allocator;
        ctx->vec = vec;
        ctx->pos = 0;

        struct kitsune_dynamic_iterator iter = kitsune_dynamic_iterator_init(
            kitsune_vec_dynamic_iterator_next,
            kitsune_vec_dynamic_iterator_previous,
            kitsune_vec_dynamic_iterator_deletor);

        iter.base.direction = SUBSTRACTION;
        kitsune_dynamic_iterator_set_context(&iter, ctx);

        return iter;
}

static void*
kitsune_vec_dynamic_iterator_next(struct kitsune_dynamic_iterator *iter)
{ 
        struct vec_iter_ctx *ctx = iter->context;
        void *item = kitsune_vec_at(ctx->vec, ctx->pos);

        if (iter->base.direction == ADDITION)
                ctx->pos++;
        else {
                if (ctx->pos == 0)
                        return NULL;
                else
                        ctx->pos--;
        }

        if (item == NULL)
                ctx->pos--;

        return item;
}

static void*
kitsune_vec_dynamic_iterator_previous(struct kitsune_dynamic_iterator *iter)
{
        struct vec_iter_ctx *ctx = iter->context;
        void *item = kitsune_vec_at(ctx->vec, ctx->pos);

        if (iter->base.direction == ADDITION) {
                if (ctx->pos == 0)
                        return NULL;
                else
                        ctx->pos--;
        } else
                ctx->pos++;

        if (item == NULL)
                ctx->pos--;

        return item;
}

static void
kitsune_vec_dynamic_iterator_deletor(struct kitsune_dynamic_iterator *iter)
{
        struct vec_iter_ctx *ctx = iter->context;
        ctx->allocator->free(ctx->allocator, iter->context);
}

#undef VEC_CHUNK
