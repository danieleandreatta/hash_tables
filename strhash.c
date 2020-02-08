//vim: et ai sw=2 ts=2

#include <string.h>

static unsigned strhash0(char *x, unsigned h, unsigned a) {
  unsigned i;
  int len = strlen(x);
  for (i = 0; i < len; ++i) {
    h = ((h * a) + x[i]);
  }
  return h;
}

unsigned strhash(char *x) {
  return strhash0(x, 0, 31);
}

unsigned strhash2(char *x) {
  return strhash0(x, 5381, 33);
}

unsigned strhash3(char *x) {
  return strhash0(x, 0, 5);
}

unsigned strhash4(char *str)
{
  unsigned hash = 0;
  int c;

  while ((c = *str++)) {
    hash = c + (hash << 6) + (hash << 16) - hash;
  }

  return hash;
}


