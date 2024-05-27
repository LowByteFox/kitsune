#include <numbers.h>
#include <generator.h>
#include <iterator.h>
#include <allocator.h>
#include <alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <assert.h>

struct generator_data {
        struct kitsune_allocator *a;
        int number;
};

void*
generate(struct kitsune_generator *iter)
{
        struct generator_data *d = iter->context;
        d->number++;
        return &d->number;
}

void
deletor(struct kitsune_generator *iter)
{
        struct generator_data *d = iter->context;
        d->a->free(d->a, iter->context);
}

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_generator iter = kitsune_generator_init(
            sizeof(int), true, a, generate, deletor);

        struct generator_data *d = a->alloc(a, sizeof(struct generator_data));
        d->a = a;
        /* Just so first number is 0 */
        d->number = -1;
        kitsune_generator_set_context(&iter, d);

        while (true) {
                int *val = kitsune_iterator_next(&iter.base);
                printf("%d\n", *val);

                if (*val == 25)
                        break;
        }
        printf("Now we go back!\n");

        int *prev = kitsune_iterator_previous(&iter.base);
        while (prev != NULL) {
            printf("%d\n", *prev);
            prev = kitsune_iterator_previous(&iter.base);
        }

        kitsune_generator_deinit(&iter, NULL);
        assert(iter.result_cache.size == 0);
        return 0;
}
