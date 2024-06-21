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

#include <locale.h>
#include <stdio.h>
#include <kitsune/numbers.h>
#include <kitsune/utf8.h>

int
main()
{
        setlocale(LC_ALL, "");
        u8 *str = (u8*) "Hello! すし";

        while (*str != 0) {
                usize len;
                kitsune_rune rune = kitsune_rune_decode(str, &len);
                printf("%lc", rune);
                str += len;
        }
        putchar(10);

        return 0;
}
