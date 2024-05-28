#ifndef _MEMUTILS_H_
#define _MEMUTILS_H_

#include <allocator.h>
#include <numbers.h>

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
