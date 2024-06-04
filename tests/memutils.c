#include <memutils.h>
#include <assert.h>

int
#ifdef ENABLE_RT
async_main()
#else
main()
#endif
{
        char buffer[6 + 3] = "Hello";

        kitsune_rmemcpy(buffer + 3, 9, buffer, 6);
        assert(kitsune_memcmp(buffer, 9, buffer, 9) == 0);

        return 0;
}
