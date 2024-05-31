#ifndef _RC_H_
#define _RC_H_

#include <allocator.h>
#include <numbers.h>

/*
 * Please, do not take ownership of the data, you will cause undefined behavior
 */
struct kitsune_rc {
        struct kitsune_allocator *allocator;
        void (*deletor)(struct kitsune_allocator*, void*);
        void *data;
        usize count;
};

/*
 * The pointer is expected to be already allocated
 */
struct kitsune_rc  *kitsune_rc_init(void*, struct kitsune_allocator*);
void                kitsune_rc_deinit(struct kitsune_rc*);
void                kitsune_rc_set_deletor(struct kitsune_rc*,
                        kitsune_allocator_deletor*);
/*
 * Use this function whenever you assign kitsune_rc to another variable
 * When you want to return kitsune_rc, you don't need to deinit it
 * But deinit it every single time you get it
 */
struct kitsune_rc  *kitsune_rc_clone(struct kitsune_rc*);

#endif
