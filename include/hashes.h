#ifndef _HASHES_H_
#define _HASHES_H_

#include <numbers.h>

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
