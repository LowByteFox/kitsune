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

#include <kitsune/hashes.h>
#include <kitsune/numbers.h>

u32
kitsune_fnv1a_32(const void *data, usize size)
{
        const u8 *buffer = data;
        u32 hash = FNV_32_OFFSET;
        usize i = 0;

        for (; i < size; i++) {
                hash ^= buffer[i];
                hash *= FNV_32_PRIME;
        }

        return hash;
}

u64
kitsune_fnv1a_64(const void *data, usize size)
{
        const u8 *buffer = data;
        u64 hash = FNV_64_OFFSET;
        usize i = 0;

        for (; i < size; i++) {
                hash ^= buffer[i];
                hash *= FNV_64_PRIME;
        }

        return hash;
}

u32
kitsune_djb2_32(const void *data, usize size)
{
        const u8 *buffer = data;
        u32 hash = 5381;
        usize i = 0;

        for (; i < size; i++) {
                hash = ((hash << 5) + hash) + buffer[i];
        }

        return hash;
}

u64
kitsune_djb2_64(const void *data, usize size)
{
        const u8 *buffer = data;
        u64 hash = 5381;
        usize i = 0;

        for (; i < size; i++) {
                hash = ((hash << 5) + hash) + buffer[i];
        }

        return hash;
}

u32
kitsune_sdbm_32(const void *data, usize size)
{
        const u8 *buffer = data;
        u32 hash = 0;
        usize i = 0;

        for (; i < size; i++) {
                hash = buffer[i] + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
}

u64
kitsune_sdbm_64(const void *data, usize size)
{
        const u8 *buffer = data;
        u64 hash = 0;
        usize i = 0;

        for (; i < size; i++) {
                hash = buffer[i] + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
}
