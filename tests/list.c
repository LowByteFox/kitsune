#include <numbers.h>
#include <iterator.h>
#include <dynamic_iterator.h>
#include <allocator.h>
#include <alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <list.h>
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
#ifdef ENABLE_RT
async_main()
#else
main()
#endif
{
        struct kitsune_allocator *a = kitsune_hardened_allocator;

        struct kitsune_list list = kitsune_list_init(sizeof(int), a);
        int val = 7;
        kitsune_list_push_back(&list, &val);
        val = 4;
        kitsune_list_push_back(&list, &val);
        val = 1;
        kitsune_list_push_back(&list, &val);
        val = 15;
        kitsune_list_push_back(&list, &val);

        struct kitsune_dynamic_iterator iter = kitsune_list_iterator(&list,
            false);

        iterate(&iter);
        val = 17;
        kitsune_list_push_back(&list, &val);
        printf("Added 17\n");
        int *current = kitsune_iterator_next(&iter.base);
        if (current != NULL)
                printf("%d\n", *current);

        current = kitsune_iterator_previous(&iter.base);
        if (current != NULL)
                printf("%d\n", *current);

        kitsune_dynamic_iterator_deinit(&iter);
        kitsune_list_deinit(&list, NULL);
        assert(kitsune_list_empty(&list) == true);
        return 0;
}
