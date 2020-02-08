#include <stdlib.h>
#include <string.h>

#include "key_list.h"
#include "new.h"
#include "debug.h"

struct _key_list_t {
  int size;
  char **keys;
};

key_list_t *keys_new(int n) {
  int i;
  key_list_t *kl = new0(key_list_t);
  kl->size = n;
  kl->keys = new1(char*, n);
  for (i = 0; i < n; ++i) kl->keys[i] = NULL;
  return kl;
}

int keys_size(key_list_t *kl) {
  return kl->size;
}

void keys_set(key_list_t *kl, int n, char *k) {
  if (n < kl->size) {
    if (kl->keys[n] != NULL) {
      free(kl->keys[n]);
    }
    kl->keys[n] = strdup(k);
  }
}

char *keys_get(key_list_t *kl, int n) {
  if (n > kl->size) return NULL;
  return kl->keys[n];
}

void keys_free(key_list_t *kl) {
  int i;
  for (i = 0; i < kl->size; ++i) {
    free(kl->keys[i]);
  }
  free(kl);
}
