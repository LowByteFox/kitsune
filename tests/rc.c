#include <kitsune/allocator.h>
#include <kitsune/alloc/hardened.h>
#include <stdio.h>
#include <kitsune/rc.h>
#include <assert.h>

void
deletor(struct kitsune_allocator *a, void *data)
{
        printf("%d is being deleted!\n", *(int*) data);
        a->free(a, data);
}

struct kitsune_rc*
givemerc(struct kitsune_allocator *a)
{
        int *data = a->alloc(a, sizeof(int));
        *data = 77;
        struct kitsune_rc *rc = kitsune_rc_init(data, a);

        return rc;
}

struct kitsune_rc*
test(struct kitsune_allocator *a)
{
        struct kitsune_rc* rc = givemerc(a);
        struct kitsune_rc* clone = kitsune_rc_clone(rc);
        assert(rc->count == 2);

        kitsune_rc_deinit(rc);

        assert(rc->count == 1);

        kitsune_rc_set_deletor(clone, deletor);

        printf("%d\n", *(int*) rc->data);

        return clone;
}

int
main()
{
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        kitsune_rc_deinit(test(a));
        /* 
         * Memory should not leak
         * Also the rc shall not be used as it will deinit itself!
         */
        return 0;
}
