#include <allocator.h>
#include <alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <deque.h>
#include <assert.h>

int
main()
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
        kitsune_deque_insert(&deq, 3, &val);

        printf("Front %d\n", *(int*) kitsune_deque_front(&deq));
        printf("Back %d\n", *(int*) kitsune_deque_back(&deq));

        kitsune_deque_pop_front(&deq);
        printf("Front after %d\n", *(int*) kitsune_deque_front(&deq));
        kitsune_deque_pop_back(&deq);
        printf("Back after %d\n", *(int*) kitsune_deque_back(&deq));

        kitsune_deque_deinit(&deq, NULL);
        return 0;
}
