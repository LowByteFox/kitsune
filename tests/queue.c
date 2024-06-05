#include <kitsune/crashtrace.h>
#include <kitsune/numbers.h>
#include <stdio.h>
#include <kitsune/queue.h>
#include <kitsune/allocator.h>
#include <kitsune/alloc/hardened.h>
#include <assert.h>

int
main()
{
        kitsune_install_crashtrace();
        struct kitsune_allocator *a = kitsune_hardened_allocator;
        struct kitsune_queue queue = kitsune_queue_init(sizeof(int), a);
        int val = 0;
        kitsune_queue_push(&queue, &val);
        val = 1;
        kitsune_queue_push(&queue, &val);
        val = 2;
        kitsune_queue_push(&queue, &val);
        val = 3;
        kitsune_queue_push(&queue, &val);

        assert(*(int*) kitsune_queue_front(&queue) == 0);
        assert(*(int*) kitsune_queue_back(&queue) == 3);
        assert(kitsune_queue_size(&queue) == 4);

        a->free(a, kitsune_queue_pop(&queue));
        int *item = kitsune_queue_pop(&queue);
        
        printf("q: ");
        for (; item != NULL; a->free(a, item)) {
                printf("%d ", *item);
                item = kitsune_queue_pop(&queue);
        }
        putchar(10);

        kitsune_queue_deinit(&queue, NULL);
        return 0;
}
