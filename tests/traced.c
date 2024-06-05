#include <kitsune/alloc/hardened.h>
#include <kitsune/allocator.h>
#include <kitsune/alloc/traced.h>

int
main()
{
        struct kitsune_allocator *a = kitsune_hardened_allocator;
        struct kitsune_traced_allocator gpa = kitsune_traced_allocator_init(a);

        struct kitsune_allocator *allocator = 
            kitsune_traced_allocator_allocator(&gpa);

        int *hehe = allocator->alloc(allocator, sizeof(int) * 2);
        hehe[0] = 4;
        hehe[1] = 3;

        /*
         * Lets leave this out :P
         * allocator->free(allocator, hehe);
         */

        kitsune_traced_allocator_deinit(&gpa);
        return 0;
}
