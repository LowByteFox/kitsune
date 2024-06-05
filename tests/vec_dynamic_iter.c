#include <kitsune/iterator.h>
#include <kitsune/dynamic_iterator.h>
#include <kitsune/allocator.h>
#include <kitsune/alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <kitsune/vec.h>
#include <assert.h>

void
iterate(struct kitsune_dynamic_iterator *iter)
{
        int *current = kitsune_iterator_next(&iter->base);
        while (current != NULL) {
                printf("%d\n", *current);
                current = kitsune_iterator_next(&iter->base);
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

        struct kitsune_dynamic_iterator iter = kitsune_vec_dynamic_iterator(
            &vec);

        iterate(&iter);
        val = 17;
        kitsune_vec_push(&vec, &val);
        printf("Added 17\n");
        iterate(&iter);

        kitsune_dynamic_iterator_deinit(&iter);
        kitsune_vec_deinit(&vec, NULL);
        assert(kitsune_vec_empty(&vec) == true);
        return 0;
}
