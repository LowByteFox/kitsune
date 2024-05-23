#include <alloc/hardened.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MV(ptr, op) ((unsigned char*) ptr) op sizeof(size_t)
#define CANARY_SIZE 100

static char CLEAN_CANARY[CANARY_SIZE];

static void *
allocate(struct kitsune_allocator *a, size_t size)
{
        (void) a;

        void *ptr = malloc(size + sizeof(size_t) + CANARY_SIZE);
        if (ptr == NULL)
                return ptr;

        memset(((unsigned char*) ptr) + sizeof(size_t) + size, 0, CANARY_SIZE);

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

        memset(((unsigned char*) orig) + sizeof(size_t) + size, 0, CANARY_SIZE);

        *(size_t*) orig = size;
        
        return MV(orig, +);
}

static void
check_pointer(void *ptr)
{
        if (memcmp(ptr, CLEAN_CANARY, CANARY_SIZE) != 0) {
                fprintf(stderr, "*** heap smashing detected ***: terminated\n");
                abort();
        }
}

static void
destroy(struct kitsune_allocator *a, void *ptr)
{
        (void) a;

        if (ptr == NULL)
                return;
        void *orig = MV(ptr, -);

        size_t size = *(size_t*) orig;
        check_pointer(((unsigned char*) orig) + sizeof(size_t) + size);

        free(orig);
}

size_t
kitsune_hardened_allocated(void *ptr)
{
        return ptr == NULL ? 0 : *(size_t*) (MV(ptr, -));
}

#undef MV

struct kitsune_allocator *const kitsune_hardened_allocator =
    (struct kitsune_allocator[]) { (struct kitsune_allocator) {
    .alloc = allocate, .resize = reallocate, .free = destroy, 
    .context = NULL } };
