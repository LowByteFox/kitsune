/*
 * Copyright (C) 2024 LowByteFox
 * This file is part of kitsune.
 *
 * kitsune is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3.0 of the License, or
 * (at your option) any later version.
 *
 * kitsune is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with kitsune; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _RC_H_
#define _RC_H_

#include <kitsune/allocator.h>
#include <kitsune/numbers.h>

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
