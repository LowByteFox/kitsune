#include <allocator.h>
#include <alloc/basic.h>
#include <stdio.h>

int
main()
{
        struct allocator *const a = kitsune_basic_allocator;
        int *arr = a->alloc(a, sizeof(int) * 2);

        arr[0] = 4;
        arr[1] = 3;

        printf("%d + %d = %d\n", arr[0], arr[1], arr[0] + arr[1]);

        a->free(a, arr);
        return 0;
}
