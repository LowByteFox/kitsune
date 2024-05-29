#include "alloc/traced.h"
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
main()
{
        struct kitsune_allocator *a = kitsune_hardened_allocator;
        struct kitsune_traced_allocator gpa = kitsune_traced_allocator_init(a);

        struct kitsune_allocator *allocator = 
            kitsune_traced_allocator_allocator(&gpa);

        struct kitsune_list list = kitsune_list_init(sizeof(int), allocator);
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
        iterate(&iter);

        kitsune_dynamic_iterator_deinit(&iter);
        kitsune_list_deinit(&list, NULL);
        assert(kitsune_list_empty(&list) == true);
        kitsune_traced_allocator_deinit(&gpa);
        return 0;
}
