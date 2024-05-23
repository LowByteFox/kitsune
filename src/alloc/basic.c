#include <alloc/basic.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#define MV(ptr, op) ((unsigned char*) ptr) op sizeof(size_t)

static void *
allocate(struct kitsune_allocator *a, size_t size)
{
        (void) a;

        void *ptr = malloc(size + sizeof(size_t));
        if (ptr == NULL)
                return ptr;

        *(size_t*) ptr = size;
        return MV(ptr, +);
}

static void *
reallocate(struct kitsune_allocator *a, void *ptr, size_t size)
{
        (void) a;

        if (ptr == NULL)
                return NULL;
        void *orig = MV(ptr, -);

        orig = realloc(orig, size + sizeof(size_t));
        if (orig == NULL)
                return NULL;

        *(size_t*) orig = size;
        
        return MV(orig, +);
}

static void
destroy(struct kitsune_allocator *a, void *ptr)
{
        (void) a;

        if (ptr == NULL)
                return;
        void *orig = MV(ptr, -);
        free(orig);
}

size_t
kitsune_basic_allocated(void *ptr)
{
        return ptr == NULL ? 0 : *(size_t*) (MV(ptr, -));
}

#undef MV

struct kitsune_allocator *const kitsune_basic_allocator =
    (struct kitsune_allocator[]) { (struct kitsune_allocator) {
    .alloc = allocate, .resize = reallocate, .free = destroy, 
    .context = NULL } };
