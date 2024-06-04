#include <locale.h>
#include <stdio.h>
#include <numbers.h>
#include <utf8.h>

int
#ifdef ENABLE_RT
async_main()
#else
main()
#endif
{
        setlocale(LC_ALL, "");
        u8 *str = (u8*) "Hello! すし";

        while (*str != 0) {
                usize len;
                kitsune_rune rune = kitsune_rune_decode(str, &len);
                printf("%lc", rune);
                str += len;
        }
        putchar(10);

        return 0;
}
