#include <allocator.h>

void *
kitsune_allocator_alloc(struct allocator *const a, size_t size)
{
        return a->alloc(a, size);
}

void *
kitsune_allocator_resize(struct allocator *const a, void *ptr, size_t size)
{
        return a->resize(a, ptr, size);
}

void 
kitsune_allocator_free(struct allocator *const a, void *ptr)
{
        a->free(a, ptr);
}
