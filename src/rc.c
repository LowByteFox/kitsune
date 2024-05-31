#include <allocator.h>
#include <rc.h>

struct kitsune_rc*
kitsune_rc_init(void *data, struct kitsune_allocator *allocator)
{
        struct kitsune_rc *rc = allocator->alloc(allocator,
            sizeof(struct kitsune_rc));

        rc->allocator = allocator;
        rc->data = data;
        rc->count = 1;
        rc->deletor = NULL;

        return rc;
}

void
kitsune_rc_deinit(struct kitsune_rc *rc)
{
        rc->count--;

        if (rc->count == 0) {
                rc->deletor != NULL ? rc->deletor(rc->allocator, rc->data) :
                    rc->allocator->free(rc->allocator, rc->data);
                rc->allocator->free(rc->allocator, rc);
        }
}

void
kitsune_rc_set_deletor(struct kitsune_rc *rc, kitsune_allocator_deletor *deletor)
{
        rc->deletor = deletor;
}

struct kitsune_rc*
kitsune_rc_clone(struct kitsune_rc *rc)
{
        rc->count++;
        return rc;
}
