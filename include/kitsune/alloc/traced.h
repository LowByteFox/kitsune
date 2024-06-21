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

#ifndef _TRACED_ALLOCATOR_H_
#define _TRACED_ALLOCATOR_H_

#include <kitsune/allocator.h>
#include <kitsune/numbers.h>

struct kitsune_traced_allocator {
        struct kitsune_allocator *base;
        /* This allocator will be generated */
        struct kitsune_allocator allocator;
};

struct kitsune_traced_allocator kitsune_traced_allocator_init(
                                    struct kitsune_allocator*);
void                            kitsune_traced_allocator_deinit(
                                    struct kitsune_traced_allocator*);
struct kitsune_allocator       *kitsune_traced_allocator_allocator(
                                    struct kitsune_traced_allocator*);

#endif
