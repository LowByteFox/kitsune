#ifndef _CRASHTRACE_H_
#define _CRASHTRACE_H_

#include <kitsune/numbers.h>

void    kitsune_install_crashtrace();
void    kitsune_print_backtrace(usize, void**, bool);

#endif
