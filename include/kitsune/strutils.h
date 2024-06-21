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

#ifndef _STRUTILS_H_
#define _STRUTILS_H_

#include <kitsune/allocator.h>
#include <kitsune/numbers.h>
#include <strings.h>

/*
 * strtok and index functions are not available as they are deprecated by other
 * functions!
 */

char   *kitsune_stpcpy(char*, const char*, usize);
usize   kitsune_strcat(char*, const char*, usize);
usize	kitsune_strcpy(char*, const char*, usize);
char   *kitsune_strdup(struct kitsune_allocator*, char*, usize);
char   *kitsune_strchr(const char*, i32, usize);
char   *kitsune_strrchr(const char*, i32, usize);
char   *kitsune_strchrnul(const char*, i32, usize);
i32     kitsune_strcmp(const char*, const char*, usize);
i32     kitsune_strcoll(const char*, const char*);
i32     kitsune_strcoll_l(const char*, const char*, locale_t);
i32     kitsune_strcasecmp(const char*, const char*, usize);
i32     kitsune_strcasecmp_l(const char*, const char*, usize, locale_t);
char   *kitsune_strerror(i32);
char   *kitsune_strerror_l(i32, locale_t);
i32     kitsune_strerror_r(i32, char*, usize);
i32     kitsune_strerror_rl(i32, char*, usize, locale_t);
usize   kitsune_strlen(const char*, usize);
char   *kitsune_strpbrk(const char*, const char*, usize);
char   *kitsune_strsep(char**, const char*, usize*);
usize   kitsune_strspn(const char*, const char*, usize);
usize   kitsune_strcspn(const char*, const char*, usize);
char   *kitsune_strstr(const char*, const char*, usize);
char   *kitsune_strcasestr(const char*, const char*, usize);
char   *kitsune_strcasestr_l(const char*, const char*, usize, locale_t);
i32	kitsune_strmode(i32, char*, usize);
char   *kitsune_strsignal(i32);
usize   kitsune_strxfrm(char*, const char*, usize);
usize   kitsune_strxfrm_l(char*, const char*, usize, locale_t);

#endif
