#include <ctype.h>
#include <kitsune/strutils.h>
#include <kitsune/utf8.h>
#include <kitsune/numbers.h>
#include <kitsune/memutils.h>
#include <kitsune/str.h>
#include <string.h>

struct kitsune_str
kitsune_str_init(const char *orig, struct kitsune_allocator *allocator)
{
	struct kitsune_str str = {0};
	str.allocator = allocator;
	str.type = STR;
	if (orig != NULL) {
		str.len = strlen(orig);
		str.str = kitsune_memdup(orig, str.len + 1, allocator);
	} else if (allocator == NULL) {
		str.len = strlen(orig);
		str.str = (char*) orig;
	}

	return str;
}

void
kitsune_str_deinit(struct kitsune_str *str)
{
	str->allocator->free(str->allocator, str->str);
}

char*
kitsune_str_as_ascii(struct kitsune_str *str)
{
	switch (str->type) {
	case STR:
		return str->str;
	default:
		return NULL;
	}
}

u8*
kitsune_str_as_bytes(struct kitsune_str *str)
{
	switch (str->type) {
	case STR:
		return (u8*) str->str;
	default:
		return NULL;
	}
}

bool
kitsune_str_contains(struct kitsune_str *str, struct kitsune_str *str2)
{
	switch (str->type) {
	case STR:
		switch (str2->type) {
			case STR:
				return kitsune_strstr(str->str, str2->str,
				    str->len) != NULL;
			default:
				return false;
		}
	default:
		return false;
	}
}

bool
kitsune_str_starts_with(struct kitsune_str *str, struct kitsune_str *str2)
{
	switch (str->type) {
	case STR:
		switch (str2->type) {
			case STR:
				return kitsune_strcmp(str->str, str2->str,
				    str2->len);
			default:
				return false;
		}
	default:
		return false;
	}
}

bool
kitsune_str_ends_with(struct kitsune_str *str, struct kitsune_str *str2)
{
	char *p;

	switch (str->type) {
	case STR:
		switch (str2->type) {
			case STR:
				p = str->str + str->len;
				while (*str2->str != 0) {
					if (*p == 0)
						return false;
					if (*p-- != *str2->str++)
						return false;
				}
				return true;
			default:
				return false;
		}
	default:
		return false;
	}
}

usize
kitsune_str_find(struct kitsune_str *str, struct kitsune_str *str2)
{
	char *ptr;

	switch (str->type) {
	case STR:
		switch (str2->type) {
			case STR:
				ptr = kitsune_strstr(str->str, str2->str,
				    str->len);

				if (ptr == NULL)
					return 0;
				return ptr - str->str;
			default:
				return 0;
		}
	default:
		return 0;
	}
}

kitsune_rune
kitsune_str_get(struct kitsune_str *str, usize index)
{
	switch (str->type) {
	case STR:
		if (str->len < index)
			return 0;
		return str->str[index];
	default:
		return 0;
	}
}

bool
kitsune_str_is_ascii(struct kitsune_str *str)
{
	switch (str->type) {
	case STR:
		return true;
	default:
		return false;
	}
}

usize
kitsune_str_length(struct kitsune_str *str)
{
	switch (str->type) {
	case STR:
		return str->len;
	default:
		return 0;
	}
}

void
kitsune_str_upper(struct kitsune_str *str)
{
	usize i = 0;
	switch (str->type) {
	case STR:
		for (; i < str->len; i++)
			str->str[i] = toupper(str->str[i]);
		break;
	}
}

void
kitsune_str_lower(struct kitsune_str *str)
{
	usize i = 0;
	switch (str->type) {
	case STR:
		for (; i < str->len; i++)
			str->str[i] = tolower(str->str[i]);
		break;
	}
}

struct kitsune_str
kitsune_str_repeat(struct kitsune_str *str, usize count)
{
	char *s;
	usize i, final;

	switch (str->type) {
	case STR:
		final = str->len * count;
		s = str->allocator->alloc(str->allocator, final + 1);
		for (i = 0; i < count; i++) {
			usize out = kitsune_strcat(s, str->str, final);
			if (out >= str->len * count + 1) {
				break;
			}
			final -= str->len;
		}
		{
			struct kitsune_str s2 = kitsune_str_init(s, NULL);
			s2.allocator = str->allocator;
			return s2;
		}
	default:
		return kitsune_str_init(NULL, str->allocator);
	}
}

void
kitsune_str_strip_prefix(struct kitsune_str *str, struct kitsune_str *str2)
{
	switch (str->type) {
	case STR:
		switch (str2->type) {
			case STR:
				if (kitsune_str_starts_with(str, str2)) {
					kitsune_memcpy2(str->str,
					    str->str + str2->len,
					    str->len - str2->len + 1);
					str->len -= str2->len;
				}
		}
	}
}
