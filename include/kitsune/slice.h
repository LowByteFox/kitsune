#ifndef _SLICE_H_
#define _SLICE_H_

#include <kitsune/numbers.h>

struct kitsune_slice {
        void *ptr;
        usize len;
};

#endif 
