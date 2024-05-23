#include <alloc/basic.h>
#include <numbers.h>

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#define MV(ptr, op) ((u8*) ptr) op sizeof(usize)

static void *
allocate(struct kitsune_allocator *a, usize size)
{
        (void) a;

        void *ptr = malloc(size + sizeof(usize));
        if (ptr == NULL)
                return ptr;

        *(usize*) ptr = size;
        return MV(ptr, +);
}

static void *
reallocate(struct kitsune_allocator *a, void *ptr, usize size)
{
        (void) a;

        if (ptr == NULL)
                return NULL;
        void *orig = MV(ptr, -);

        orig = realloc(orig, size + sizeof(usize));
        if (orig == NULL)
                return NULL;

        *(usize*) orig = size;
        
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

usize
kitsune_basic_allocated(void *ptr)
{
        return ptr == NULL ? 0 : *(usize*) (MV(ptr, -));
}

#undef MV

struct kitsune_allocator *const kitsune_basic_allocator =
    (struct kitsune_allocator[]) { (struct kitsune_allocator) {
    .alloc = allocate, .resize = reallocate, .free = destroy, 
    .context = NULL } };
