#include <iterator.h>
#include <dynamic_iterator.h>
#include <numbers.h>
#include <allocator.h>
#include <alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <deque.h>
#include <assert.h>

int
#ifdef ENABLE_RT
async_main()
#else
main()
#endif
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_deque deq = kitsune_deque_init(sizeof(int), a);

        int val = 10;
        kitsune_deque_push_front(&deq, &val);
        val = 20;
        kitsune_deque_push_front(&deq, &val);
        val = 30;
        kitsune_deque_push_back(&deq, &val);
        val = 40;
        kitsune_deque_push_back(&deq, &val);
        val = 5;
        kitsune_deque_insert(&deq, 3, &val); /* 20, 10, 30, 5, 40 */

        struct kitsune_dynamic_iterator iter = kitsune_deque_iterator(&deq);

        int *item = kitsune_iterator_next(&iter.base);
        while (item != NULL) {
                printf("%d ", *item);
                item = kitsune_iterator_next(&iter.base);
        }
        putchar(10);

        kitsune_dynamic_iterator_deinit(&iter);
        kitsune_deque_deinit(&deq, NULL);

        assert(kitsune_deque_empty(&deq) == true);
        return 0;
}
