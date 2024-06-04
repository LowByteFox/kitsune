#include <allocator.h>
#include <alloc/basic.h>
#include <assert.h>

int
#ifdef ENABLE_RT
async_main()
#else
main()
#endif
{
        struct kitsune_allocator *const a = kitsune_basic_allocator;
        int *arr = a->alloc(a, sizeof(int) * 2);

        arr[0] = 4;
        arr[1] = 3;

        assert(kitsune_allocated(arr) == sizeof(int) * 2);
        struct kitsune_pointer *ptr = kitsune_visualize(arr);
        assert(ptr->size == sizeof(int) * 2);

        a->free(a, arr);

        return 0;
}
