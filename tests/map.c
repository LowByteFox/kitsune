#include <crashtrace.h>
#include <numbers.h>
#include <allocator.h>
#include <alloc/basic.h>
#include <stdio.h>
#include <strings.h>
#include <map.h>
#include <hashes.h>
#include <assert.h>

int
main()
{
        kitsune_install_crashtrace();
        struct kitsune_allocator *const a = kitsune_basic_allocator;
        struct kitsune_map map = kitsune_map_init(sizeof(usize), a,
            kitsune_fnv1a_64);
        usize i = 0;

        for (; i < 10000; i++) {
            kitsune_map_insert(&map, &i, sizeof(usize), &i);
        }

        for (i = 0; i < 10000; i++) {
            usize *value = kitsune_map_get(&map, &i, sizeof(usize));
            printf("%ld\n", *value);
        }

        kitsune_map_deinit(&map, NULL);
        assert(kitsune_map_empty(&map) == true);
        return 0;
}
