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
