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

#ifndef _MEMUTILS_H_
#define _MEMUTILS_H_

#include <kitsune/allocator.h>
#include <kitsune/numbers.h>

/* Safer variants of ISO functions */
void   *kitsune_memchr(const void *, usize, i32, usize);
void   *kitsune_memcpy(void*, usize, const void*, usize);
void   *kitsune_memmove(void*, usize, const void*, usize);
void   *kitsune_memset(void*, usize, i32, usize);
i32     kitsune_memcmp(const void*, usize, const void*, usize);

/* Reverse order */
void   *kitsune_rmemcpy(void*, usize, const void*, usize);

/* ISO functions for kitsune_allocator allocated pointers */
void   *kitsune_memchr2(const void *, i32, usize);
void   *kitsune_memcpy2(void*, const void*, usize);
void   *kitsune_memmove2(void*, const void*, usize);
void   *kitsune_memset2(void*, i32, usize);
i32     kitsune_memcmp2(const void*, const void*, usize);

/* Reverse order */
void   *kitsune_rmemcpy2(void*, const void*, usize);

/* Custom functions */
void   *kitsune_memdup(const void*, usize, struct kitsune_allocator*);

/* Custom functions for kitsune_allocator allocated pointers */
void   *kitsune_memdup2(const void*, struct kitsune_allocator*);

#endif
