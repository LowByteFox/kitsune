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

#include <strings.h>
#include <kitsune/vec.h>
#include <kitsune/iterator.h>
#include <kitsune/dynamic_iterator.h>
#include <kitsune/numbers.h>
#include <kitsune/allocator.h>
#include <kitsune/deque.h>

static void                        *kitsune_deque_iterator_next(
                                        struct kitsune_dynamic_iterator*);
static void                        *kitsune_deque_iterator_previous(
                                        struct kitsune_dynamic_iterator*);
static void                         kitsune_deque_iterator_deletor(
                                        struct kitsune_dynamic_iterator*);

struct deque_iter_ctx {
        struct kitsune_allocator *allocator;
        struct kitsune_deque *deque;
        usize pos;
};

struct kitsune_deque
kitsune_deque_init(usize chunksize, struct kitsune_allocator *allocator)
{
        struct kitsune_deque deque = {0};
        deque.back = kitsune_vec_init(chunksize, allocator);
        deque.front = kitsune_vec_init(chunksize, allocator);
        deque.allocator = allocator;
        deque.size = 0;

        return deque;
}

void
kitsune_deque_deinit(struct kitsune_deque *deque,
    kitsune_allocator_deletor *deletor)
{
        kitsune_vec_deinit(&deque->front, deletor);
        kitsune_vec_deinit(&deque->back, deletor);
        deque->size = 0;
}

void
kitsune_deque_push_back(struct kitsune_deque *deque, void *data)
{
        kitsune_vec_push(&deque->back, data);
        deque->size++;
}

void
kitsune_deque_push_front(struct kitsune_deque *deque, void *data)
{
        kitsune_vec_push(&deque->front, data);
        deque->size++;
}

void
kitsune_deque_insert(struct kitsune_deque *deque, usize index, void *data)
{
        if (index < deque->front.size) {
                index = deque->front.size - 1 - index;
                kitsune_vec_insert(&deque->front, index, data);
        } else
                kitsune_vec_insert(&deque->back, index - deque->front.size,
                    data);

        deque->size++;
}

void*
kitsune_deque_back(struct kitsune_deque *deque)
{
        if (deque->size == 0)
                return NULL;

        if (deque->back.size > 0)
                return kitsune_vec_get(&deque->back, deque->back.size - 1);
        else if (deque->front.size > 0)
                return kitsune_vec_get(&deque->front, 0);

        return NULL;
}

void*
kitsune_deque_front(struct kitsune_deque *deque)
{
        if (deque->size == 0)
                return NULL;

        if (deque->front.size > 0)
                return kitsune_vec_get(&deque->front, deque->front.size - 1);
        else if (deque->back.size > 0)
                return kitsune_vec_get(&deque->back, 0);

        return NULL;
}

void*
kitsune_deque_pop_back(struct kitsune_deque *deque)
{
        if (deque->size == 0)
                return NULL;

        deque->size--;

        if (deque->back.size == 0) {
                while (deque->front.size > 0) {
                        void *item = kitsune_vec_pop(&deque->front);
                        kitsune_vec_push(&deque->back, item);
                        deque->allocator->free(deque->allocator, item);
                }
        }
        return kitsune_vec_pop(&deque->back);
}

void*
kitsune_deque_pop_front(struct kitsune_deque *deque)
{
        if (deque->size == 0)
                return NULL;

        deque->size--;

        if (deque->front.size == 0) {
                while (deque->back.size > 0) {
                        void *item = kitsune_vec_pop(&deque->back);
                        kitsune_vec_push(&deque->front, item);
                        deque->allocator->free(deque->allocator, item);
                }
        }
        return kitsune_vec_pop(&deque->front);
}

void*
kitsune_deque_remove(struct kitsune_deque *deque, usize index)
{
        if (deque->size == 0)
                return NULL;

        deque->size--;
        if (index < deque->front.size) {
                index = deque->front.size - 1 - index;
                return kitsune_vec_remove(&deque->front, index);
        } else
                return kitsune_vec_remove(&deque->back,
                    index - deque->front.size);
}

void*
kitsune_deque_get(struct kitsune_deque *deque, usize index)
{
        if (index < deque->front.size) {
                index = deque->front.size - 1 - index;
                return kitsune_vec_get(&deque->front, index);
        } else
                return kitsune_vec_get(&deque->back, index - deque->front.size);
}

usize
kitsune_deque_size(struct kitsune_deque *deque)
{
        return deque->size;
}

bool
kitsune_deque_empty(struct kitsune_deque *deque)
{
        return deque->size == 0;
}

struct kitsune_dynamic_iterator
kitsune_deque_iterator(struct kitsune_deque *deque)
{
        struct deque_iter_ctx *ctx = deque->allocator->alloc(deque->allocator,
            sizeof(struct deque_iter_ctx));
        ctx->allocator = deque->allocator;
        ctx->deque = deque;
        ctx->pos = 0;

        struct kitsune_dynamic_iterator iter = kitsune_dynamic_iterator_init(
            kitsune_deque_iterator_next,
            kitsune_deque_iterator_previous,
            kitsune_deque_iterator_deletor);
        kitsune_dynamic_iterator_set_context(&iter, ctx);

        return iter;
}

struct kitsune_dynamic_iterator
kitsune_deque_reverse_iterator(struct kitsune_deque *deque)
{
        struct deque_iter_ctx *ctx = deque->allocator->alloc(deque->allocator,
            sizeof(struct deque_iter_ctx));
        ctx->allocator = deque->allocator;
        ctx->deque = deque;
        ctx->pos = 0;

        struct kitsune_dynamic_iterator iter = kitsune_dynamic_iterator_init(
            kitsune_deque_iterator_next,
            kitsune_deque_iterator_previous,
            kitsune_deque_iterator_deletor);
        iter.base.direction = SUBSTRACTION;
        kitsune_dynamic_iterator_set_context(&iter, ctx);

        return iter;
}

static void*
kitsune_deque_iterator_next(struct kitsune_dynamic_iterator *iter)
{
        struct deque_iter_ctx *ctx = iter->context;
        void *item = kitsune_deque_get(ctx->deque, ctx->pos);
        
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
kitsune_deque_iterator_previous(struct kitsune_dynamic_iterator *iter)
{
        struct deque_iter_ctx *ctx = iter->context;
        void *item = kitsune_deque_get(ctx->deque, ctx->pos);

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
kitsune_deque_iterator_deletor(struct kitsune_dynamic_iterator *iter)
{
        struct deque_iter_ctx *ctx = iter->context;
        ctx->allocator->free(ctx->allocator, iter->context);
}
