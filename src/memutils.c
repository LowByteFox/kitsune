#include <assert.h>
#include <numbers.h>
#include <allocator.h>
#include <memutils.h>

void *
kitsune_memchr(const void *ptr, usize destsize, i32 c, usize n)
{
        if (ptr == NULL)
                return NULL;
        assert(destsize >= n);

        const u8 *p = ptr;
        u8 ch = c;

        while (n--) {
                if (*p == ch) {
                        return (void *) p;
                }
                p++;
        }

        return NULL;
}

void *
kitsune_memcpy(void *dest, usize destsize, const void *src, usize n)
{
        if (dest == NULL || src == NULL)
                return NULL;

        assert(destsize >= n);

        u8 *d = dest;
        const u8 *s = src;

        while (n--) {
            *d++ = *s++;
        }

        return dest;
}

void *
kitsune_memmove(void *dest, usize destsize, const void *src, usize n)
{
        if (dest == NULL || src == NULL)
                return NULL;

        assert(destsize >= n);

        u8 *d = dest;
        const u8 *s = src;

        if (d < s) {
                while (n--) {
                        *d++ = *s++;
                }
        } else {
                d += n;
                s += n;
                while (n--) {
                        *--d = *--s;
                }
        }

        return dest;
}

void *
kitsune_memset(void *dest, usize destsize, i32 c, usize n)
{
        if (dest == NULL)
                return NULL;

        assert(destsize >= n);

        u8 *d = dest;
        u8 ch = c;

        while (n--) {
            *d++ = ch;
        }

        return dest;
}

i32
kitsune_memcmp(const void *dest, usize destsize, const void *src, usize n)
{
        if (dest == NULL || src == NULL)
                return -1;

        assert(destsize >= n);

        const u8 *d = dest;
        const u8 *s = src;

        while (n--) {
                if (*d != *s) {
                        return *d - *s;
                }

                d++;
                s++;
        }

        return 0;

}

void *
kitsune_rmemcpy(void *dest, usize destsize, const void *src, usize n)
{
        if (dest == NULL || src == NULL)
                return NULL;

        assert(destsize >= n);

        u8 *d = dest;
        const u8 *s = src;

        d += n - 1;
        s += n - 1;

        while (n--) {
            *d-- = *s--;
        }

        return dest;
}

void *
kitsune_memchr2(const void *ptr, i32 c, usize n)
{
        if (ptr == NULL)
                return NULL;
        assert(kitsune_allocated(ptr) >= n);

        const u8 *p = ptr;
        u8 ch = c;

        while (n--) {
                if (*p == ch) {
                        return (void *) p;
                }
                p++;
        }

        return NULL;
}

void *
kitsune_memcpy2(void *dest, const void *src, usize n)
{
        if (dest == NULL || src == NULL)
                return NULL;

        assert(kitsune_allocated(dest) >= n);

        u8 *d = dest;
        const u8 *s = src;

        while (n--) {
            *d++ = *s++;
        }

        return dest;
}

void *
kitsune_memmove2(void *dest, const void *src, usize n)
{
        if (dest == NULL || src == NULL)
                return NULL;

        assert(kitsune_allocated(dest) >= n);

        u8 *d = dest;
        const u8 *s = src;

        if (d < s) {
                while (n--) {
                        *d++ = *s++;
                }
        } else {
                d += n;
                s += n;
                while (n--) {
                        *--d = *--s;
                }
        }

        return dest;
}

void *
kitsune_memset2(void *dest, i32 c, usize n)
{
        if (dest == NULL)
                return NULL;

        assert(kitsune_allocated(dest) >= n);

        u8 *d = dest;
        u8 ch = c;

        while (n--) {
            *d++ = ch;
        }

        return dest;
}

i32
kitsune_memcmp2(const void *dest, const void *src, usize n)
{
        if (dest == NULL || src == NULL)
                return -1;

        assert(kitsune_allocated(dest) >= n);

        const u8 *d = dest;
        const u8 *s = src;

        while (n--) {
                if (*d != *s) {
                        return *d - *s;
                }

                d++;
                s++;
        }

        return 0;

}

void *
kitsune_rmemcpy2(void *dest, const void *src, usize n)
{
        if (dest == NULL || src == NULL)
                return NULL;

        assert(kitsune_allocated(dest) >= n);

        u8 *d = dest;
        const u8 *s = src;

        d += n - 1;
        s += n - 1;

        while (n--) {
            *d-- = *s--;
        }

        return dest;
}
