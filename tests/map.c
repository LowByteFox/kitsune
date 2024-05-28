#include <dynamic_iterator.h>
#include <iterator.h>
#include <crashtrace.h>
#include <allocator.h>
#include <alloc/hardened.h>
#include <stdio.h>
#include <strings.h>
#include <map.h>
#include <hashes.h>
#include <assert.h>

int
main()
{
        kitsune_install_crashtrace();
        struct kitsune_allocator *const a = kitsune_hardened_allocator;
        struct kitsune_map map = kitsune_map_init(sizeof(int), a,
            kitsune_fnv1a_64);

        int val = 7;
        kitsune_map_insert(&map, "Hello!", 7, &val);
        val = 50;
        kitsune_map_insert(&map, "Hey!", 5, &val);
        val = 10;
        kitsune_map_insert(&map, "uwu!", 5, &val);
        val = 4;
        kitsune_map_insert(&map, "owo", 4, &val);

        struct kitsune_dynamic_iterator iter = kitsune_map_iterator(&map);
        struct kitsune_map_entry *item = kitsune_iterator_next(&iter.base);

        while (item != NULL) {
                printf("%s\t=> %d\n", (char*) item->key, *(int*) item->value);
                item = kitsune_iterator_next(&iter.base);
        }

        kitsune_map_deinit(&map, NULL);
        assert(kitsune_map_empty(&map) == true);
        return 0;
}
