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

#ifndef _LIST_H_
#define _LIST_H_

#include <kitsune/dynamic_iterator.h>
#include <kitsune/allocator.h>
#include <kitsune/numbers.h>

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

struct kitsune_list kitsune_list_init(usize, struct kitsune_allocator*);
void                kitsune_list_deinit(struct kitsune_list*,
                        kitsune_allocator_deletor*);
void                kitsune_list_append(struct kitsune_list*,
                        struct kitsune_list*);
void                kitsune_list_push_back(struct kitsune_list*, void*);
void                kitsune_list_push_front(struct kitsune_list*, void*);
void                kitsune_list_insert(struct kitsune_list*, usize, void*);
void               *kitsune_list_back(struct kitsune_list*);
void               *kitsune_list_front(struct kitsune_list*);
/*
 * BOTH POP AND REMOVE return ALLCATED memory by the linked list!
 * Don't forget to free it!
 */
void               *kitsune_list_pop_back(struct kitsune_list*);
void               *kitsune_list_pop_front(struct kitsune_list*);
void               *kitsune_list_remove(struct kitsune_list*, usize);
void               *kitsune_list_get(struct kitsune_list*, usize);
bool                kitsune_list_contains(struct kitsune_list*, void*);
usize               kitsune_list_size(struct kitsune_list*);
void                kitsune_list_resize(struct kitsune_list*, usize, void*,
                        kitsune_allocator_deletor*);
bool                kitsune_list_empty(struct kitsune_list*);

/* Iterator API */
struct kitsune_dynamic_iterator kitsune_list_iterator(struct kitsune_list*,
                                    bool);
struct kitsune_dynamic_iterator kitsune_list_reverse_iterator(
                                    struct kitsune_list*, bool);

void                            kitsune_list_iterator_set_circular(
                                    struct kitsune_dynamic_iterator*, bool);

#endif
