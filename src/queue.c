#include "queue.h"
#include "list.h"
#include "numbers.h"

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
