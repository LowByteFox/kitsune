#ifndef _UTF8_H_
#define _UTF8_H_

#include <numbers.h>

typedef i32 kitsune_rune;

usize kitsune_rune_length(u8);
kitsune_rune kitsune_rune_decode(const u8*, usize*);
usize kitsune_rune_encode(kitsune_rune, u8*);

#endif
