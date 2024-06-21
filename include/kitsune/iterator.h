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

#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include <kitsune/numbers.h>

/* Used to tell the direction of memory for static iterator */
enum kitsune_iterator_direction {
        ADDITION,
        SUBSTRACTION
};

enum kitsune_iterator_type {
        STATIC,
        GENERATOR,
        DYNAMIC
};

/* 
 * Simple static iterator
 * Results don't need to be cached as everything is known and already in the 
 * memory
 *
 * Doesn't need to be deinitilized
 */
struct kitsune_iterator {
        void *begin;
        void *current;
        usize chunk;
        void *end;
        enum kitsune_iterator_direction direction;
        enum kitsune_iterator_type kind;
};

/* Iterator API, generator and dynamic iterator compatible */
struct kitsune_iterator kitsune_iterator_init(void*, void*, usize);
void                    kitsune_iterator_change_direction(
                            struct kitsune_iterator*,
                            enum kitsune_iterator_direction);
void                   *kitsune_iterator_next(struct kitsune_iterator*);
void                   *kitsune_iterator_previous(struct kitsune_iterator*);

#endif
