#ifndef __DEBUG__
#define __DEBUG__

#ifdef DEBUG

#include <stdio.h>
#define debug(...) printf (__VA_ARGS__)
#else
#define debug(...)
#endif

#endif
