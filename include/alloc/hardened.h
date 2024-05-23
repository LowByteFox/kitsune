#ifndef _HARDENED_ALLOCATOR_H_
#define _HARDENED_ALLOCATOR_H_

#include <allocator.h>

extern struct kitsune_allocator *const kitsune_hardened_allocator;

/* This function returns how many bytes are allocated on the pointer */
size_t  kitsune_hardened_allocated(void*);

#endif
