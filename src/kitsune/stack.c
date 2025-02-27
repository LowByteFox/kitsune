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

#include <kitsune/stack.h>
#include <kitsune/numbers.h>
#include <kitsune/memutils.h>
#include <kitsune/allocator.h>

#define STACK_CHUNK 16

static usize    kitsune_stack_capacity(struct kitsune_stack*);
static void     kitsune_stack_reserve(struct kitsune_stack*, usize);
static void     kitsune_stack_shrink(struct kitsune_stack*);
static void    *kitsune_stack_begin(struct kitsune_stack*);
static void    *kitsune_stack_end(struct kitsune_stack*);
static void    *kitsune_stack_rbegin(struct kitsune_stack*);

struct kitsune_stack
kitsune_stack_init(usize chunksize, struct kitsune_allocator *allocator)
{
    struct kitsune_stack stack = {0};
    stack.chunksize = chunksize;
    stack.allocator = allocator;

    return stack;
}

void
kitsune_stack_deinit(struct kitsune_stack *stack,
    kitsune_allocator_deletor *deletor)
{
        void *iter = kitsune_stack_begin(stack);
        void *end = kitsune_stack_end(stack);

        while (iter != end) {
                deletor ? deletor(stack->allocator, iter) : NULL;
                iter = ((u8*) iter) + stack->chunksize;
        }

        stack->allocator->free(stack->allocator, stack->items);
        stack->items = NULL;
        stack->size = 0;
}

void
kitsune_stack_push(struct kitsune_stack *stack, void *data)
{
        usize capacity = kitsune_stack_capacity(stack);
        if (stack->size == capacity)
                kitsune_stack_reserve(stack, capacity + STACK_CHUNK);
        stack->size++;

        u8 *ptr = stack->items;
        kitsune_memcpy(ptr + (stack->size - 1) * stack->chunksize, stack->chunksize, 
            data, stack->chunksize);
}

void*
kitsune_stack_pop(struct kitsune_stack *stack)
{
        if (stack->size == 0)
                return NULL;

        void *data = stack->allocator->alloc(stack->allocator, stack->chunksize);
        kitsune_memcpy(data, stack->chunksize, kitsune_stack_rbegin(stack),
            stack->chunksize);

        stack->size--;

        usize capacity = kitsune_stack_capacity(stack);
        if (stack->size + STACK_CHUNK == capacity)
                kitsune_stack_shrink(stack);
        return data;
}

void*
kitsune_stack_top(struct kitsune_stack *stack)
{
        if (stack->size == 0)
                return NULL;

        return kitsune_stack_rbegin(stack);
}

usize
kitsune_stack_size(struct kitsune_stack *stack)
{
        return stack->size;
}

bool
kitsune_stack_empty(struct kitsune_stack *stack)
{
        return stack->size == 0;
}

static void*
kitsune_stack_begin(struct kitsune_stack *stack)
{
        return stack->items;
}

static void*
kitsune_stack_end(struct kitsune_stack *stack)
{
        return ((u8*) stack->items) + stack->chunksize * stack->size;
}

static void*
kitsune_stack_rbegin(struct kitsune_stack *stack)
{
        return stack->size == 0 ? kitsune_stack_end(stack) : ((u8*) stack->items) +
            stack->chunksize * (stack->size - 1);
}

static usize
kitsune_stack_capacity(struct kitsune_stack *stack)
{
        usize allocated = kitsune_allocated(stack->items);
        return allocated / stack->chunksize;
}

static void
kitsune_stack_reserve(struct kitsune_stack *stack, usize new_capacity)
{
        usize capacity = kitsune_stack_capacity(stack);
        if (new_capacity <= capacity)
                return;

        stack->items = stack->allocator->resize(stack->allocator, stack->items,
            new_capacity * stack->chunksize);
}

static void
kitsune_stack_shrink(struct kitsune_stack *stack)
{
        usize allocated = kitsune_allocated(stack->items);
        if (allocated == stack->size * stack->chunksize)
                return;

        stack->items = stack->allocator->resize(stack->allocator, stack->items,
            stack->size * stack->chunksize);
}
