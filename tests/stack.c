#include <allocator.h>
#include <alloc/hardened.h>
#include <stdio.h>
#include <stack.h>
#include <assert.h>

int
#ifdef ENABLE_RT
async_main()
#else
main()
#endif
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_stack stack = kitsune_stack_init(sizeof(int), a);
        int i;
        for (i = 0; i < 10; i++) {
                kitsune_stack_push(&stack, &i);
        }
        assert(kitsune_stack_size(&stack) == 10);
        assert(*(int*) kitsune_stack_top(&stack) == 9);

        for (i = 0; i < 10; i++) {
                int *val = kitsune_stack_pop(&stack);
                printf("%d\n", *val);
                a->free(a, val);
        }

        kitsune_stack_deinit(&stack, NULL);
        assert(kitsune_stack_empty(&stack) == true);
        return 0;
}
