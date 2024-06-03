#ifndef _STRING_H_
#define _STRING_H_

#include <iterator.h>
#include <dynamic_iterator.h>
#include <_str.h>
#include <utf8.h>
#include <allocator.h>
#include <numbers.h>

struct kitsune_string {
        struct kitsune_allocator *allocator;
        usize length;
        kitsune_rune *data;
};

struct kitsune_string   kitsune_string_init(const char*,
                            struct kitsune_allocator*);
void                    kitsune_string_deinit(struct kitsune_string*);
void                    kitsune_string_push(struct kitsune_string*,
                            kitsune_rune);
void                    kitsune_string_push_str(struct kitsune_string*,
                            struct kitsune_str*);
void                    kitsune_string_insert(struct kitsune_string*, usize,
                            kitsune_rune);
kitsune_rune            kitsune_string_pop(struct kitsune_string*);
kitsune_rune            kitsune_string_remove(struct kitsune_string*, usize);
void                    kitsune_string_append(struct kitsune_string*, usize,
                            kitsune_rune);
usize                   kitsune_string_capacity(struct kitsune_string*);
void                    kitsune_string_reserve(struct kitsune_string*, usize);
void                    kitsune_string_shrink_to_fit(struct kitsune_string*);
bool                    kitsune_string_empty(struct kitsune_string*);

/* Str API */
char                   *kitsune_string_as_ascii(struct kitsune_string*);

/* Iterator API */
struct kitsune_iterator kitsune_string_chars(struct kitsune_string*);
struct kitsune_dynamic_iterator kitsune_string_dynamic_chars(
                                    struct kitsune_string*);

#endif
