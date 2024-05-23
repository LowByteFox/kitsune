#include <alloc/hardened.h>
#include <numbers.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MV(ptr, op) ((u8*) ptr) op sizeof(usize)
#define CANARY_SIZE 100

static char CLEAN_CANARY[CANARY_SIZE];

static void *
allocate(struct kitsune_allocator *a, usize size)
{
        (void) a;

        void *ptr = malloc(size + sizeof(usize) + CANARY_SIZE);
        if (ptr == NULL)
                return ptr;

        memset(((u8*) ptr) + sizeof(usize) + size, 0, CANARY_SIZE);

        *(usize*) ptr = size;
        return MV(ptr, +);
}

static void *
reallocate(struct kitsune_allocator *a, void *ptr, usize size)
{
        (void) a;

        void *orig = NULL;

        if (ptr != NULL)
                orig = MV(ptr, -);

        orig = realloc(orig, size + sizeof(usize));
        if (orig == NULL)
                return NULL;

        memset(((u8*) orig) + sizeof(usize) + size, 0, CANARY_SIZE);

        *(usize*) orig = size;
        
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

        usize size = *(size_t*) orig;
        check_pointer(((u8*) orig) + sizeof(usize) + size);

        free(orig);
}

#undef MV

struct kitsune_allocator *const kitsune_hardened_allocator =
    (struct kitsune_allocator[]) { (struct kitsune_allocator) {
    .alloc = allocate, .resize = reallocate, .free = destroy, 
    .context = NULL } };
