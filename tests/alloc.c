#include <allocator.h>
#include <alloc/basic.h>
#include <assert.h>

int
main()
{
        struct kitsune_allocator *const a = kitsune_basic_allocator;
        int *arr = a->alloc(a, sizeof(int) * 2);

        arr[0] = 4;
        arr[1] = 3;

        assert(kitsune_basic_allocated(arr) == sizeof(int) * 2);

        a->free(a, arr);

        return 0;
}
