#ifndef _STACK_H_
#define _STACK_H_

#include <allocator.h>
#include <numbers.h>

struct kitsune_stack {
    struct kitsune_allocator *allocator;
    usize chunksize;
    void *items;
    usize size;
};

#endif
