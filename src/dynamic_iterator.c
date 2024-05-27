#include <iterator.h>
#include <dynamic_iterator.h>

struct kitsune_dynamic_iterator
kitsune_dynamic_iterator_init(kitsune_dynamic_iterator_fn *next,
    kitsune_dynamic_iterator_fn *previous,
    kitsune_dynamic_iterator_deletor *deletor)
{
        struct kitsune_dynamic_iterator iter = {0};
        iter.base = kitsune_iterator_init(NULL, NULL, 0);
        iter.base.kind = DYNAMIC;
        iter.next = next;
        iter.previous = previous;
        iter.deletor = deletor;

        return iter;
}

void
kitsune_dynamic_iterator_set_context(struct kitsune_dynamic_iterator *iter,
    void *context)
{
        iter->context = context;
}

void
kitsune_dynamic_iterator_deinit(struct kitsune_dynamic_iterator *iter)
{
        iter->deletor != NULL ? iter->deletor(iter) : NULL;
}
