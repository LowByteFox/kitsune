#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <kitsune/numbers.h>

/* Structural visualization of kitsune pointer */
struct kitsune_pointer {
        usize size;
        u8 ptr[];
};

struct kitsune_allocator {
        void *(*alloc)(struct kitsune_allocator *const, usize);
        void *context;
        void (*free)(struct kitsune_allocator *const, void*);
        void *(*resize)(struct kitsune_allocator *const, void*, usize);
};

typedef void kitsune_allocator_deletor(struct kitsune_allocator*, void*);

void   *kitsune_allocator_alloc(struct kitsune_allocator *const, usize);
void   *kitsune_allocator_resize(struct kitsune_allocator *const, void*, 
            usize);
void    kitsune_allocator_free(struct kitsune_allocator *const, void*);
usize   kitsune_allocated(const void*);


struct kitsune_pointer *kitsune_visualize(void*);

#endif
