#include <iterator.h>
#include <allocator.h>
#include <alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <vec.h>
#include <assert.h>

void
iterate(struct kitsune_iterator *i)
{
        int *current = kitsune_iterator_next(i);
        while (current != NULL) {
                printf("%d\n", *current);
                current = kitsune_iterator_next(i);
        }
}

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_vec vec = kitsune_vec_init(sizeof(int), a);
        int val = 7;
        kitsune_vec_push(&vec, &val);
        val = 4;
        kitsune_vec_push(&vec, &val);
        val = 1;
        kitsune_vec_push(&vec, &val);
        val = 15;
        kitsune_vec_push(&vec, &val);

        struct kitsune_iterator iter = kitsune_vec_iterator(&vec);
        iterate(&iter);
        printf("Now reverse!\n");
        iter = kitsune_vec_reverse_iterator(&vec);
        iterate(&iter);

        kitsune_vec_deinit(&vec, NULL);
        assert(kitsune_vec_empty(&vec) == true);
        return 0;
}
