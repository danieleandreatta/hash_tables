#ifndef __NEW__
#define __NEW__

#include <stdlib.h>

#define new0(t) (t*)malloc(sizeof(t))
#define new1(t, n) (t*)calloc(n, sizeof(t))

#endif
