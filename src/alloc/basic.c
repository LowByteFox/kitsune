#include <alloc/basic.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

static void *
allocate(struct allocator *a, size_t size)
{
        (void) a;

        return malloc(size);
}

static void *
reallocate(struct allocator *a, void *ptr, size_t size)
{
        (void) a;

        return realloc(ptr, size);
}

static void
destroy(struct allocator *a, void *ptr)
{
        (void) a;

        free(ptr);
}

struct allocator *const kitsune_basic_allocator = (struct allocator[]) {
        (struct allocator) { .alloc = allocate, .resize = reallocate,
            .free = destroy, .context = NULL }
};
