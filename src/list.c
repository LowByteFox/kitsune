#include <memutils.h>
#include <assert.h>
#include <list.h>
#include <allocator.h>
#include <numbers.h>
#include <iterator.h>
#include <dynamic_iterator.h>

static struct kitsune_list_node    *kitsune_list_new_node(struct kitsune_list*,
                                        void*);
static void                        *kitsune_list_iterator_next(
                                        struct kitsune_dynamic_iterator*);
static void                        *kitsune_list_iterator_previous(
                                        struct kitsune_dynamic_iterator*);
static void                         kitsune_list_iterator_deletor(
                                        struct kitsune_dynamic_iterator*);

struct list_iter_ctx {
        struct kitsune_allocator *allocator;
        struct kitsune_list_node *current;
        struct kitsune_list_node *previous;
        struct kitsune_list *list;
        bool circular;
};

struct kitsune_list
kitsune_list_init(usize datasize, struct kitsune_allocator *allocator)
{
        struct kitsune_list list = {0};
        list.allocator = allocator;
        list.datasize = datasize;
        
        return list;
}

void
kitsune_list_deinit(struct kitsune_list *list, kitsune_allocator_deletor *deletor)
{
        struct kitsune_list_node *current = list->head;
        struct kitsune_list_node *next = NULL;

        while (current != NULL) {
                next = current->next;
                deletor != NULL ? deletor(list->allocator, current->data)
                    : list->allocator->free(list->allocator, current->data);

                list->allocator->free(list->allocator, current);
                current = next;
        }
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
}

void
kitsune_list_append(struct kitsune_list *list, struct kitsune_list *list2) 
{
        assert(list->allocator == list2->allocator);
        assert(list->datasize == list2->datasize);

        list->tail->next = list2->head;
        list->tail = list2->tail;
        list->size += list2->size;

        list2->size = 0;
        list2->head = NULL;
        list2->tail = NULL;
}

void
kitsune_list_push_back(struct kitsune_list *list, void *data)
{
        struct kitsune_list_node *node = kitsune_list_new_node(list, data);
        if (list->head == NULL) {
                list->head = node;
                list->tail = node;
                return;
        }
        node->previous = list->tail;
        list->tail->next = node;
        list->tail = node;
}

void
kitsune_list_push_front(struct kitsune_list *list, void *data)
{
        struct kitsune_list_node *node = kitsune_list_new_node(list, data);
        if (list->head == NULL) {
                list->head = node;
                list->tail = node;
                return;
        }
        node->next = list->head;
        list->head->previous = node;
        list->head = node;
}

void*
kitsune_list_back(struct kitsune_list *list)
{
        if (list->tail == NULL)
                return NULL;

        return list->tail->data;
}

void*
kitsune_list_front(struct kitsune_list *list)
{
        if (list->head == NULL)
                return NULL;

        return list->head->data;
}

void
kitsune_list_insert(struct kitsune_list *list, usize index, void *data)
{
        if (index >= list->size) {
                kitsune_list_push_back(list, data);
                return;
        }
        usize i = 0;

        struct kitsune_list_node *current = list->head;
        for (; i < index; i++) {
                current = current->next;
        }
        struct kitsune_list_node *node = kitsune_list_new_node(list, data);
        node->previous = current;
        node->next = current->next;
        current->next = node;
}

void*
kitsune_list_pop_back(struct kitsune_list *list)
{
        struct kitsune_list_node *node = list->tail;
        list->tail = node->previous;

        void *data = node->data;
        list->allocator->free(list->allocator, node);
        list->size--;

        return data;
}

void*
kitsune_list_pop_front(struct kitsune_list *list)
{
        struct kitsune_list_node *node = list->head;
        list->head = node->next;

        void *data = node->data;
        list->allocator->free(list->allocator, node);
        list->size--;

        return data;
}

void*
kitsune_list_remove(struct kitsune_list *list, usize index)
{
        if (index >= list->size)
                return kitsune_list_pop_back(list);

        usize i = 0;

        struct kitsune_list_node *current = list->head;
        for (; i < index; i++) {
                current = current->next;
        }

        current->previous->next = current->next;
        current->next->previous = current->previous;

        void *data = current->data;
        list->allocator->free(list->allocator, current);
        list->size--;

        return data;
}

void*
kitsune_list_at(struct kitsune_list *list, usize index)
{
        if (list->size <= index) return NULL;
        usize i = 0;

        struct kitsune_list_node *current = list->head;
        for (; i < index; i++) {
                current = current->next;
        }

        return current->data;
}

bool
kitsune_list_contains(struct kitsune_list *list, void *data)
{
        usize i = 0;

        struct kitsune_list_node *current = list->head;
        for (; i < list->size; i++) {
                if (kitsune_memcmp2(current->data, data, list->datasize) == 0)
                        return true;
                current = current->next;
        }

        return false;
}

usize
kitsune_list_size(struct kitsune_list *list)
{
        return list->size;
}

void
kitsune_list_resize(struct kitsune_list *list, usize size, void *def,
    kitsune_allocator_deletor *deletor)
{
        if (size > list->size) {
                usize i = 0;
                usize count = size - list->size;
                for (; i < count; i++) {
                        kitsune_list_push_back(list, def);
                }
        } else if (size < list->size) {
                usize i = 0;
                usize count = list->size - size;
                for (; i < count; i++) {
                        void *data = kitsune_list_pop_back(list);
                        deletor != NULL ? deletor(list->allocator, data)
                            : NULL;
                }
        }
}

bool
kitsune_list_empty(struct kitsune_list *list)
{
        return list->size == 0;
}

struct kitsune_dynamic_iterator
kitsune_list_iterator(struct kitsune_list *list, bool circular)
{
        struct list_iter_ctx *ctx = list->allocator->alloc(list->allocator,
            sizeof(struct list_iter_ctx));
        ctx->allocator = list->allocator;
        ctx->list = list;
        ctx->current = list->head;
        ctx->previous = list->head;
        ctx->circular = circular;

        struct kitsune_dynamic_iterator iter = kitsune_dynamic_iterator_init(
            kitsune_list_iterator_next,
            kitsune_list_iterator_previous,
            kitsune_list_iterator_deletor);
        kitsune_dynamic_iterator_set_context(&iter, ctx);

        return iter;
}

struct kitsune_dynamic_iterator
kitsune_list_reverse_iterator(struct kitsune_list *list, bool circular)
{
        struct list_iter_ctx *ctx = list->allocator->alloc(list->allocator,
            sizeof(struct list_iter_ctx));
        ctx->allocator = list->allocator;
        ctx->list = list;
        ctx->current = list->tail;
        ctx->previous = list->tail;
        ctx->circular = circular;

        struct kitsune_dynamic_iterator iter = kitsune_dynamic_iterator_init(
            kitsune_list_iterator_next,
            kitsune_list_iterator_previous,
            kitsune_list_iterator_deletor);

        iter.base.direction = SUBSTRACTION;
        kitsune_dynamic_iterator_set_context(&iter, ctx);

        return iter;
}

void
kitsune_list_iterator_set_circular(struct kitsune_dynamic_iterator *iter,
    bool circular)
{
        assert(iter != NULL);

        struct list_iter_ctx *ctx = iter->context;
        ctx->circular = circular;
}

static struct kitsune_list_node*
kitsune_list_new_node(struct kitsune_list *list, void *data)
{
        struct kitsune_list_node *node = list->allocator->alloc(
            list->allocator, sizeof(struct kitsune_list_node));
        node->data = kitsune_memdup(data, list->datasize, list->allocator);
        node->next = NULL;
        node->previous = NULL;
        list->size++;

        return node;
}

static void*
kitsune_list_iterator_next(struct kitsune_dynamic_iterator *iter)
{
        struct list_iter_ctx *ctx = iter->context;
        if (iter->base.direction == ADDITION) {
                if (ctx->current == NULL && ctx->previous->next == NULL)
                        return NULL;
                else if (ctx->current == NULL && ctx->previous->next != NULL) {
                        ctx->current = ctx->previous->next;
                        ctx->previous = ctx->current;
                }
        } else {
                if (ctx->current == NULL && ctx->previous->previous == NULL)
                        return NULL;
                else if (ctx->current == NULL && ctx->previous->previous !=
                    NULL) {
                        ctx->current = ctx->previous->previous;
                        ctx->previous = ctx->current;
                }
        }

        void *item = ctx->previous->data;

        if (iter->base.direction == ADDITION) {
                if (ctx->current->next == NULL) {
                        ctx->current = ctx->current->next;
                        if (ctx->circular) {
                                ctx->current = ctx->list->head;
                                ctx->previous = ctx->current;
                        } else
                                return item;
                } else {
                        ctx->current = ctx->current->next;
                        ctx->previous = ctx->current;
                }
        } else {
                if (ctx->current->previous == NULL) {
                        ctx->current = ctx->list->tail;
                        if (ctx->circular) {
                                ctx->current = ctx->list->tail; 
                                ctx->previous = ctx->current;
                        } else
                                return item;
                } else {
                        ctx->current = ctx->current->previous;
                        ctx->previous = ctx->current;
                }
        }

        return item;
}

static void*
kitsune_list_iterator_previous(struct kitsune_dynamic_iterator *iter)
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

        void *item = kitsune_list_iterator_next(iter);
        iter->base.direction = dir;

        return item;
}

static void
kitsune_list_iterator_deletor(struct kitsune_dynamic_iterator *iter)
{
        struct list_iter_ctx *ctx = iter->context;
        ctx->allocator->free(ctx->allocator, iter->context);
}
