#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <stddef.h>

struct allocator {
        void *(*alloc)(struct allocator *const, size_t);
        void *(*resize)(struct allocator *const, void*, size_t);
        void (*free)(struct allocator *const, void*);
        void *context;
};

void    *kitsune_allocator_alloc(struct allocator *const, size_t);
void    *kitsune_allocator_resize(struct allocator *const, void*, size_t);
void    kitsune_allocator_free(struct allocator *const, void*);

#endif
