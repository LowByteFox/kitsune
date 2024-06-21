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

#include <kitsune/queue.h>
#include <kitsune/list.h>
#include <kitsune/numbers.h>

struct kitsune_queue
kitsune_queue_init(usize chunksize, struct kitsune_allocator *allocator)
{
        struct kitsune_queue queue = {0};
        queue.list = kitsune_list_init(chunksize, allocator);
        return queue;
}

void
kitsune_queue_deinit(struct kitsune_queue *queue,
    kitsune_allocator_deletor *deletor)
{
        kitsune_list_deinit(&queue->list, deletor);
}

void
kitsune_queue_push(struct kitsune_queue *queue, void *data)
{
        kitsune_list_push_back(&queue->list, data);
}

void*
kitsune_queue_pop(struct kitsune_queue *queue)
{
        return kitsune_list_pop_front(&queue->list);
}

void*
kitsune_queue_front(struct kitsune_queue *queue)
{
        return kitsune_list_front(&queue->list);
}

void*
kitsune_queue_back(struct kitsune_queue *queue)
{
        return kitsune_list_back(&queue->list);
}

usize
kitsune_queue_size(struct kitsune_queue *queue)
{
        return queue->list.size;
}

bool
kitsune_queue_empty(struct kitsune_queue *queue)
{
        return queue->list.size == 0;
}
