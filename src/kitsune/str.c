#include <kitsune/string.h>
#include <kitsune/str.h>
#include <kitsune/iterator.h>
#include <kitsune/dynamic_iterator.h>
#include <kitsune/utf8.h>
#include <kitsune/numbers.h>

struct kitsune_str  kitsune_str_init(char *str)
{
        struct kitsune_str s = {0};
        s.str = str;
        s.type = STR;
        return s;
}

void
kitsune_str_deinit(struct kitsune_str *str)
{
        switch (str->type) {
        case STRING: {
                struct kitsune_string *s = (void*) str;
                kitsune_string_deinit(s);
        }
        default:
            break;
        }
}

char*
kitsune_str_as_ascii(struct kitsune_str *str)
{
        switch (str->type) {
        case STRING: {
                struct kitsune_string *s = (void*) str;
                return kitsune_string_as_ascii(s);
        }
        default:
            return str->str;
        }
}

u8*
kitsune_str_as_bytes(struct kitsune_str *str)
{
        /* Only CPU instructions change, not the data */
        switch (str->type) {
        case STRING: {
                struct kitsune_string *s = (void*) str;
                return (void*) kitsune_string_as_ascii(s);
        }
        default:
            return (void*) str->str;
        }
}

bool
kitsune_str_contains(struct kitsune_str *str, struct kitsune_str *str2);

bool
kitsune_str_starts_with(struct kitsune_str *str, struct kitsune_str *str2);

bool                kitsune_str_ends_with(struct kitsune_str*,
                        struct kitsune_str*);
usize               kitsune_str_find(struct kitsune_str*, struct kitsune_str*);
usize               kitsune_str_rfind(struct kitsune_str*, struct kitsune_str*);
kitsune_rune        kitsune_str_get(struct kitsune_str*, usize);
bool                kitsune_str_is_ascii(struct kitsune_str*);
usize               kitsune_str_length(struct kitsune_str*);
void                kitsune_str_upper(struct kitsune_str*);
void                kitsune_str_lower(struct kitsune_str*);
struct kitsune_str  kitsune_str_repeat(struct kitsune_str*, usize);
void                kitsune_str_replace(struct kitsune_str*,
                        struct kitsune_str*, struct kitsune_str*);
void                kitsune_str_replacen(struct kitsune_str*,
                        struct kitsune_str*, struct kitsune_str*, usize);
void                kitsune_str_strip_prefix(struct kitsune_str*,
                        struct kitsune_str*);
void                kitsune_str_strip_suffix(struct kitsune_str*,
                        struct kitsune_str*);
void                kitsune_str_trim(struct kitsune_str*);
void                kitsune_str_trim_start(struct kitsune_str*);
void                kitsune_str_trim_end(struct kitsune_str*);

/* Iterator API */
struct kitsune_dynamic_iterator kitsune_str_split(struct kitsune_str*,
                                    struct kitsune_str*);
struct kitsune_dynamic_iterator kitsune_str_rsplit(struct kitsune_str*,
                                    struct kitsune_str*);
struct kitsune_dynamic_iterator kitsune_str_splitn(struct kitsune_str*,
                                    struct kitsune_str*, usize);
struct kitsune_dynamic_iterator kitsune_str_rsplitn(struct kitsune_str*,
                                    struct kitsune_str*, usize);
struct kitsune_iterator         kitsune_str_chars(struct kitsune_str*);

struct kitsune_dynamic_iterator kitsune_str_dynamic_chars(struct kitsune_str*);
struct kitsune_dynamic_iterator kitsune_str_lines(struct kitsune_str*);
