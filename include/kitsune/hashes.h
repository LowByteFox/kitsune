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

#ifndef _HASHES_H_
#define _HASHES_H_

#include <kitsune/numbers.h>

#define FNV_32_PRIME 16777619
#define FNV_32_OFFSET 2166136261

#define FNV_64_PRIME 1099511628211
#define FNV_64_OFFSET 14695981039346656037ULL

u32 kitsune_fnv1a_32(const void *, usize);
u64 kitsune_fnv1a_64(const void *, usize);

u32 kitsune_djb2_32(const void *, usize);
u64 kitsune_djb2_64(const void *, usize);

u32 kitsune_sdbm_32(const void *, usize);
u64 kitsune_sdbm_64(const void *, usize);

#endif
