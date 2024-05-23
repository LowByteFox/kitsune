#include <allocator.h>
#include <alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <vec.h>
#include <assert.h>

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_vec vec = kitsune_vec_init(sizeof(int), a);
        int val = 7;

        kitsune_vec_push(&vec, &val);
        val = 77;
        kitsune_vec_push(&vec, &val);
        val = 777;
        kitsune_vec_insert(&vec, 7, &val);
        kitsune_vec_insert(&vec, 2, &val);
        kitsune_vec_insert(&vec, 2, &val);
        val = 77;
        kitsune_vec_push(&vec, &val);
        val = 7;
        kitsune_vec_insert(&vec, 7, &val);
        assert(kitsune_vec_size(&vec) == 7);

        int *removed = kitsune_vec_remove(&vec, vec.size - 2);
        assert(*removed == 77);
        a->free(a, removed);
        removed = kitsune_vec_pop(&vec);
        assert(*removed == 7);
        a->free(a, removed);

        assert(kitsune_vec_size(&vec) == 5);

        int *iter = kitsune_vec_begin(&vec);
        int *end = kitsune_vec_end(&vec);

        for (; iter != end; iter++) {
            printf("%d\n", *iter);
        }

        kitsune_vec_deinit(&vec, NULL);

        assert(kitsune_vec_empty(&vec) == true);
        return 0;
}
