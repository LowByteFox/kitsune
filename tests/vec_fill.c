#include <allocator.h>
#include <alloc/basic.h>
#include <strings.h>
#include <vec.h>
#include <assert.h>

int
#ifdef ENABLE_RT
async_main()
#else
main()
#endif
{
        struct kitsune_allocator *const a = kitsune_basic_allocator;
        struct kitsune_vec vec = kitsune_vec_init(sizeof(int), a);
        int i;

        for (i = 0; i < 10000; i++) {
            kitsune_vec_push(&vec, &i);
        }

        assert(kitsune_vec_size(&vec) == 10000);

        for (i = 0; i < 10000; i++) {
            int *item = kitsune_vec_pop(&vec);
            a->free(a, item);
        }

        kitsune_vec_deinit(&vec, NULL);
        assert(kitsune_vec_empty(&vec) == true);
        return 0;
}
