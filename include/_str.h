#ifndef _STR_H_
#define _STR_H_

#include <iterator.h>
#include <dynamic_iterator.h>
#include <utf8.h>
#include <numbers.h>

enum kitsune_str_type {
        STR,
        STRING,
        STRING_VIEW
};

struct kitsune_str {
        enum kitsune_str_type type;
        char *str;
};

struct kitsune_str  kitsune_str_init(char*);
void                kitsune_str_deinit(struct kitsune_str*);
char               *kitsune_str_as_ascii(struct kitsune_str*);
u8                 *kitsune_str_as_bytes(struct kitsune_str*);
bool                kitsune_str_contains(struct kitsune_str*,
                        struct kitsune_str*);
bool                kitsune_str_starts_with(struct kitsune_str*,
                        struct kitsune_str*);
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

#endif
