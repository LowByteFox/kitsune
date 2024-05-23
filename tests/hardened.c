#include <allocator.h>
#include <alloc/hardened.h>
#include <assert.h>

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        int *arr = a->alloc(a, sizeof(int) * 2);

        arr[0] = 4;
        arr[1] = 3;

        assert(kitsune_hardened_allocated(arr) == sizeof(int) * 2);

        /* Dirty the canary */
        arr[2] = 7;

        a->free(a, arr);

        return 0;
}
