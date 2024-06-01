#include <numbers.h>
#include <utf8.h>

usize
kitsune_rune_length(u8 c)
{
        if (c < 0x80) return 1;
        else if ((c & 0xE0) == 0xC0) return 2;
        else if ((c & 0xF0) == 0xE0) return 3;
        else if ((c & 0xF8) == 0xF0) return 4;
        return 0; /* Invalid Rune */
}

kitsune_rune
kitsune_rune_decode(const u8 *buffer, usize *length)
{
        u8 c = buffer[0];
        kitsune_rune r;

        if (c < 0x80) {
                *length = 1;
                return c;
        } else if ((c & 0xE0) == 0xC0) {
                *length = 2;
                r = c & 0x1F;
        } else if ((c & 0xF0) == 0xE0) {
                *length = 3;
                r = c & 0x0F;
        } else if ((c & 0xF8) == 0xF0) {
                *length = 4;
                r = c & 0x07;
        } else {
                *length = 0; /* Invalid Rune */
                return 0;
        }

        usize i = 1;
        for (; i < *length; i++) {
                r = (r << 6) | (buffer[i] & 0x3F);
        }

        return r;
}

usize
kitsune_rune_encode(kitsune_rune rune, u8 *buffer)
{
        if (rune <= 0x7F) {
                buffer[0] = rune;
                return 1;
        } else if (rune <= 0x7FF) {
                buffer[0] = 0xC0 | (rune >> 6);
                buffer[1] = 0x80 | (rune & 0x3F);
                return 2;
        } else if (rune <= 0xFFFF) {
                buffer[0] = 0xE0 | (rune >> 12);
                buffer[1] = 0x80 | ((rune >> 6) & 0x3F);
                buffer[2] = 0x80 | (rune & 0x3F);
                return 3;
        } else if (rune <= 0x10FFFF) {
                buffer[0] = 0xF0 | (rune >> 18);
                buffer[1] = 0x80 | ((rune >> 12) & 0x3F);
                buffer[2] = 0x80 | ((rune >> 6) & 0x3F);
                buffer[3] = 0x80 | (rune & 0x3F);
                return 4;
        }

        return 0; /* Invalid Rune */
}
