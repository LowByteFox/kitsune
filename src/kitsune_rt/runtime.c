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

#include <kitsune/allocator.h>
#include <kitsune/alloc/basic.h>
#include <kitsune/coroutines.h>
#include <kitsune/numbers.h>
#include <kitsune/crashtrace.h>

i32 async_main(i32 argc, char **argv);

static i32 exit_code = 0;

struct main_data {
        i32 argc;
        char **argv;
};

static async void*
async_main_hook(void *arg)
{
        struct main_data *data = arg;
        exit_code = async_main(data->argc, data->argv);
        return NULL;
}

i32
main(i32 argc, char **argv)
{
        kitsune_install_crashtrace();
        struct kitsune_allocator *a = kitsune_basic_allocator;
        struct kitsune_runtime *rt = kitsune_runtime_init(16 * 1024, a);
        kitsune_set_global_runtime(rt);

        struct main_data data = (struct main_data) { .argc = argc, .argv = argv };

        kitsune_runtime_create_task(rt, async_main_hook, &data);
        kitsune_runtime_run(rt);

        kitsune_runtime_deinit(rt);
        return exit_code;
}
