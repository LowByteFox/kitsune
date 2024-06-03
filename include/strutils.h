#ifndef _STRUTILS_H_
#define _STRUTILS_H_

#include <numbers.h>

usize   kitsune_strcat(char*, const char*, usize);
usize   kitsune_strcpy(char*, const char*, usize);
char   *kitsune_strchr(char*, usize, int);
char   *kitsune_strrchr(char*, usize, int);


#endif
