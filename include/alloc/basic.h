#ifndef _BASIC_ALLOCATOR_H_
#define _BASIC_ALLOCATOR_H_

#include <allocator.h>
#include <numbers.h>

extern struct kitsune_allocator *const kitsune_basic_allocator;

/* This function returns how many bytes are allocated on the pointer */
usize  kitsune_basic_allocated(void*);

#endif
