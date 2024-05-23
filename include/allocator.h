#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <numbers.h>

struct kitsune_allocator {
        void *(*alloc)(struct kitsune_allocator *const, usize);
        void *(*resize)(struct kitsune_allocator *const, void*, usize);
        void (*free)(struct kitsune_allocator *const, void*);
        void *context;
};

void   *kitsune_allocator_alloc(struct kitsune_allocator *const, usize);
void   *kitsune_allocator_resize(struct kitsune_allocator *const, void*, 
            usize);
void    kitsune_allocator_free(struct kitsune_allocator *const, void*);
usize   kitsune_allocated(const void*);

#endif
