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
#include <kitsune/config.h>
#include <assert.h>
#include <kitsune/numbers.h>
#include <kitsune/memutils.h>
#include <kitsune/strutils.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

#if defined(__FreeBSD__)
#include <xlocale.h>
#endif

char*
kitsune_stpcpy(char *dst, const char *src, usize n)
{
	assert(src != NULL);
	assert(dst != NULL);

	if (n != 0) {
		char *d = dst;
		const char *s = src;

		dst = dst + n;
		do {
			if ((*d++ = *s++) == 0) {
				dst = d - 1;
				while (--n != 0)
					*d++ = 0;
				break;
			}
		} while (--n != 0);
	}

	return dst;
}

usize
kitsune_strcat(char *dst, const char *src, usize dsize)
{
	assert(src != NULL);
	assert(dst != NULL);
	assert(dsize > 0);

	const char *odst = dst;
	const char *osrc = src;
	usize n = dsize;
	usize dlen;

	/* Find the end of dst */
	while (n-- != 0 && *dst != 0)
		dst++;
	dlen = dst - odst;
	n = dsize - dlen;

	if (n-- == 0)
		return dlen + strlen(src);

	while (*src != 0) {
		if (n != 0) {
			*dst++ = *src;
			n--;
		}
		src++;
	}
	*dst = 0;

	return dlen + (src - osrc);
}

usize
kitsune_strcpy(char *dst, const char *src, usize dsize)
{
	assert(src != NULL);
	assert(dst != NULL);
	assert(dsize > 0);

	const char *osrc = src;
	usize nleft = dsize;

	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == 0)
				break;
		}
	}

	if (nleft == 0) {
		if (dsize != 0)
			*dst = 0;
		while (*src++)
			;
	}

	return src - osrc - 1;
}

char*
kitsune_strdup(struct kitsune_allocator *allocator, char *src, usize maxlen)
{
	assert(allocator != NULL);
	assert(src != NULL);

	char *copy;
	usize len;

	len = kitsune_strlen(src, maxlen);
	copy = allocator->alloc(allocator, len + 1);
	if (copy != NULL) {
		kitsune_memcpy2(copy, src, len);
		copy[len] = 0;
	}

	return copy;
}

char*
kitsune_strchr(const char *p, i32 ch, usize maxlen)
{
	assert(p != NULL);

	for (; maxlen > 0; p++, maxlen--) {
		if (*p == (char) ch)
			return (char*) p;
		if (*p == 0)
			return NULL;
	}

	return NULL;
}

char*
kitsune_strrchr(const char *p, i32 ch, usize maxlen)
{
	assert(p != NULL);

	char *save;

	for (save = NULL; maxlen > 0; p++, maxlen--) {
		if (*p == (char) ch)
			save = (char*) p;
		if (*p == 0)
			return save;
	}

	return save;
}

char*
kitsune_strchrnul(const char *p, i32 ch, usize maxlen)
{
	assert(p != NULL);

	for (; maxlen > 0; p++, maxlen--) {
		if (*p == (char) ch)
			return (char*) p;
		if (*p == 0)
			return (char*) p;
	}

	return (char*) p;
}

i32
kitsune_strcmp(const char *s1, const char *s2, usize n)
{
	assert(s1 != NULL);
	assert(s2 != NULL);

	if (n == 0)
		return 0;

	do {
		if (*s1 != *s2++)
			return *(u8*) s1 - *(u8*) --s2;
		if (*s1++ == 0)
			break;
	} while (--n != 0);

	return 0;
}

/* These APIs may not be implemented on each target
 * For example OpenBSD, hence always "C" locale
 */

i32
kitsune_strcoll(const char *s1, const char *s2)
{
	assert(s1 != NULL);
	assert(s2 != NULL);
	return strcoll(s1, s2);
}

i32
kitsune_strcoll_l(const char *s1, const char *s2, locale_t locale)
{
	assert(s1 != NULL);
	assert(s2 != NULL);
	return strcoll_l(s1, s2, locale);
}

i32
kitsune_strcasecmp(const char *s1, const char *s2, usize n)
{
	assert(s1 != NULL);
	assert(s2 != NULL);
	return strncasecmp(s1, s2, n);
}

i32
kitsune_strcasecmp_l(const char *s1, const char *s2, usize n, locale_t locale)
{
	assert(s1 != NULL);
	assert(s2 != NULL);
	return strncasecmp_l(s1, s2, n, locale);
}

char*
kitsune_strerror(i32 code)
{
	return strerror(code);
}

char*
kitsune_strerror_l(i32 code, locale_t locale)
{
	return strerror_l(code, locale);
}

i32
kitsune_strerror_r(i32 code, char *buffer, usize n)
{
	assert(buffer != NULL);
#ifdef KITSUNE_TARGET_BSD
	return strerror_r(code, buffer, n);
#elif defined(KITSUNE_TARGET_Linux)
    return strerror_r(code, buffer, n) != NULL;
#endif
}

i32
kitsune_strerror_rl(i32 code, char *buffer, usize n, locale_t locale)
{
	assert(buffer != NULL);
	char *msg = strerror_l(code, locale);
	if (msg == NULL)
		return 1;

	kitsune_memcpy(buffer, n, msg, strlen(msg));
	return 0;
}

usize
kitsune_strlen(const char *p, usize maxlen)
{
	assert(p != NULL);
	const char *cp = p;

	for (; maxlen > 0 && *cp != 0; cp++, maxlen--)
		;

	return (usize) (cp - p);
}

char*
kitsune_strpbrk(const char *s1, const char *charset, usize maxlen)
{
	assert(s1 != NULL);
	assert(charset != NULL);

	if (maxlen == 0)
		return NULL;

	const char *scanp;
	i32 c, sc;

	while((c = *s1++) != 0 && maxlen > 0) {
		for (scanp = charset; (sc = *scanp++) != 0;)
			if (sc == c)
				return (char*) (s1 - 1);
	}
	return NULL;
}

char*
kitsune_strsep(char **stringp, const char *delim, usize *maxlen)
{
	assert(maxlen != NULL);

	char *s;
	const char *spanp;
	i32 c, sc;
	char *tok;

	if ((s = *stringp) == NULL)
		return NULL;
	for (tok = s; *maxlen > 0; (*maxlen)--) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;

				*stringp = s;
				return tok;
			}
		} while (sc != 0);
	}

	return NULL;
}

usize
kitsune_strspn(const char *s1, const char *s2, usize maxlen)
{
	assert(maxlen > 0);
	const char *p = s1, *spanp;
	char c, sc;

	/* Skip any chracters in s2, excluding NUL */
cont:
	c = *p++;
	for (spanp = s2; (sc = *spanp++) != 0 && maxlen > 0; maxlen--)
		if (sc == c)
			goto cont;

	return p - 1 - s1;
}

usize
kitsune_strcspn(const char *s1, const char *s2, usize maxlen)
{
	const char *p, *spanp;
	char c, sc;

	for (p = s1; maxlen > 0; maxlen--) {
		c = *p++;
		spanp = s2;
		do {
			if ((sc = *spanp++) == c)
				return (p - 1 - s1);
		} while (sc != 0);
	}

	return 0;
}

char*
kitsune_strstr(const char *s1, const char *s2, usize maxlen)
{
	assert(s1 != NULL);
	assert(s2 != NULL);

	char *res = strstr(s1, s2);
	if (res != NULL && (usize) (res - s1) < maxlen) {
		return res;
	}

	return NULL;
}

char*
kitsune_strcasestr(const char *s, const char *find, usize maxlen)
{
	assert(s != NULL);
	assert(find != NULL);

	char *res = strcasestr(s, find);
	if (res != NULL && (usize) (res - s) < maxlen) {
		return res;
	}

	return NULL;
}

char*
kitsune_strcasestr_l(const char *s, const char *find, usize maxlen,
    locale_t locale)
{
	assert(s != NULL);
	assert(find != NULL);
	char *res = NULL;
#if defined(__OpenBSD__)
    (void) locale;
	return kitsune_strcasestr(s, find, maxlen);
#elif defined(__FreeBSD__)
	res = strcasestr_l(s, find, locale);
#elif defined(KITSUNE_TARGET_LINUX)
    (void) locale;
    return kitsune_strcasestr(s, find, maxlen);
#endif
	if (res != NULL && (usize) (res - s) < maxlen) {
		return res;
	}

	return NULL;
}

i32
kitsune_strmode(i32 mode, char *p, usize size)
{
	usize written = 1;

	if (written > size)
		return 1;

	/* print type */
	switch (mode & S_IFMT) {
	case S_IFDIR:
		*p++ = 'd';
		break;
	case S_IFCHR:
		*p++ = 'c';
		break;
	case S_IFBLK:
		*p++ = 'b';
		break;
	case S_IFREG:
		*p++ = '-';
		break;
	case S_IFLNK:
		*p++ = 'l';
		break;
	case S_IFSOCK:
		*p++ = 's';
		break;
#ifdef S_IFIFO
	case S_IFIFO:
		*p++ = 'p';
		break;
#endif
#ifdef S_IFWHT
	case S_IFWHT:
		*p++ = 'w';
		break;
#endif
	default:
		*p++ = '?';
		break;
	}
	written++;

	if (written > size)
		return 1;

	/* usr */
	if (mode & S_IRUSR)
		*p++ = 'r';
	else
		*p++ = '-';
	if (mode & S_IWUSR)
		*p++ = 'w';
	else
		*p++ = '-';
	switch (mode & (S_IXUSR | S_ISUID)) {
	case 0:
		*p++ = '-';
		break;
	case S_IXUSR:
		*p++ = 'x';
		break;
	case S_ISUID:
		*p++ = 'S';
		break;
	case S_IXUSR | S_ISUID:
		*p++ = 's';
		break;
	}
	written++;

	if (written > size)
		return 1;

	/* group */
	if (mode & S_IRGRP)
		*p++ = 'r';
	else
		*p++ = '-';
	if (mode & S_IWGRP)
		*p++ = 'w';
	else
		*p++ = '-';
	switch (mode & (S_IXGRP | S_ISGID)) {
	case 0:
		*p++ = '-';
		break;
	case S_IXGRP:
		*p++ = 'x';
		break;
	case S_ISGID:
		*p++ = 'S';
		break;
	case S_IXGRP | S_ISGID:
		*p++ = 's';
		break;
	}
	written++;

	if (written > size)
		return 1;

	/* other */
	if (mode & S_IROTH)
		*p++ = 'r';
	else
		*p++ = '-';
	if (mode & S_IWOTH)
		*p++ = 'w';
	else
		*p++ = '-';
	switch (mode & (S_IXOTH | S_ISVTX)) {
	case 0:
		*p++ = '-';
		break;
	case S_IXOTH:
		*p++ = 'x';
		break;
	case S_ISVTX:
		*p++ = 'T';
		break;
	case S_IXOTH | S_ISVTX:
		*p++ = 't';
		break;
	}
	written++;

	if (written > size)
		return 1;

	*p++ = ' ';
	written++;

	if (written > size)
		return 1;
	*p = '\0';

	return 0;
}

char*
kitsune_strsignal(i32 sig)
{
	assert(sig > 0);
	return strsignal(sig);
}

usize
kitsune_strxfrm(char *s1, const char *s2, usize maxlen)
{
	assert(s1 != NULL);
	assert(s2 != NULL);
	return strxfrm(s1, s2, maxlen);
}

usize
kitsune_strxfrm_l(char *s1, const char *s2, usize maxlen, locale_t locale)
{
	assert(s1 != NULL);
	assert(s2 != NULL);
	return strxfrm_l(s1, s2, maxlen, locale);
}
