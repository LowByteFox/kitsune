#include <numbers.h>
#include <dynamic_iterator.h>
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
generate(struct kitsune_dynamic_iterator *iter)
{
        struct generator_data *d = iter->context;
        d->number++;
        return &d->number;
}

void
deletor(struct kitsune_dynamic_iterator *iter)
{
        struct generator_data *d = iter->context;
        d->a->free(d->a, iter->context);
}

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_dynamic_iterator iter = kitsune_dynamic_iterator_init(
            sizeof(int), false, a, generate, deletor);

        struct generator_data *d = a->alloc(a, sizeof(struct generator_data));
        d->a = a;
        /* Just so first number is 0 */
        d->number = -1;
        kitsune_dynamic_iterator_set_context(&iter, d);

        while (true) {
                int *val = kitsune_iterator_next(&iter.base);
                printf("%d\n", *val);
        }

        kitsune_dynamic_iterator_deinit(&iter);
        return 0;
}
