#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <stddef.h>

struct kitsune_allocator {
        void *(*alloc)(struct kitsune_allocator *const, size_t);
        void *(*resize)(struct kitsune_allocator *const, void*, size_t);
        void (*free)(struct kitsune_allocator *const, void*);
        void *context;
};

void    *kitsune_allocator_alloc(struct kitsune_allocator *const, size_t);
void    *kitsune_allocator_resize(struct kitsune_allocator *const, void*, 
            size_t);
void    kitsune_allocator_free(struct kitsune_allocator *const, void*);

#endif
