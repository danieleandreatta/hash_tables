#include <string.h>
#include <stdlib.h>

#include "quadratic.h"

#include "strhash.h"
#include "new.h"
#include "key_list.h"
#include "debug.h"

static const int MAX_LOAD = 70;
static const int INIT_SIZE = 11;

static const int SIZES[] = {11, 17, 25, 37, 59, 89, 137, 211, 317, 479, 719, 1087, 1637, 2459, 3691, 5557, 8353, 12539, 18839, 28277, 42433, 63649, 95479, 143239, 214867, 322319, 483481, 725273, 1087937, 0};

struct _bucket_t {
  char *key;
  int del;
};

struct _quadratic_hash_t {
  int entries;
  int buckets;
  bucket_t *data;
};

static quadratic_hash_t * quadratic_new0(int n);
static void quadratic_rehash(quadratic_hash_t *h);

quadratic_hash_t * quadratic_new() {
  return quadratic_new0(INIT_SIZE);
}

void quadratic_free(quadratic_hash_t *h) {
  int i;
  for (i=0; i<h->buckets; ++i) free(h->data[i].key);
  free(h->data);
  free(h);
}

static void quadratic_add0(quadratic_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets;
  unsigned u;
  int i;
  for (i = 0; i < h->buckets; ++i) {
    u = (v + i * i) % h->buckets;
    if (h->data[u].key == NULL) {
      h->data[u].key = k;
      h->data[u].del = 0;
      ++ h->entries;
      break;
    }
  }
}

void quadratic_add(quadratic_hash_t *h, char *k) {
  if (quadratic_get(h, k) == 1) return;
  if (h->entries * 100 > h->buckets * MAX_LOAD) {
    quadratic_rehash(h);
  }
  quadratic_add0(h, strdup(k));
}

int quadratic_del(quadratic_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets;
  unsigned u;
  int i, found = 0;
  for (i = 0; i < h->buckets; ++i) {
    u = (v + i * i) % h->buckets;
    if (h->data[u].key == NULL && !h->data[u].del) break;
    if (h->data[u].key != NULL && !strcmp(h->data[u].key, k)) {
      free(h->data[u].key);
      h->data[u].key = NULL;
      h->data[u].del = 1;
      -- h->entries;
      found = 1;
      break;
    }
  }
  return found;
}

int quadratic_get(quadratic_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets;
  unsigned u;
  int i, found = 0;
  for (i = 0; i < h->buckets; ++i) {
    u = (v + i * i) % h->buckets;
    if (h->data[u].key == NULL && !h->data[u].del) break;
    if (h->data[u].key != NULL && !strcmp(h->data[u].key, k)) {
      found = 1;
      break;
    }
  }
  return found;
}

int quadratic_size(quadratic_hash_t *h) {
  return h->entries;
}

key_list_t * quadratic_keys(quadratic_hash_t *h) {
  int i, j = 0;
  key_list_t *kl = keys_new(h->entries);
  for(i = 0; i < h->buckets; ++i) {
    if (h->data[i].key != NULL && !h->data[i].del) {
      keys_set(kl, j, h->data[i].key);
      ++ j;
    }
  }
  return kl;
}

static quadratic_hash_t * quadratic_new0(int n) {
  quadratic_hash_t *h = new0(quadratic_hash_t);
  h->data = new1(bucket_t, n);
  h->buckets = n;
  h->entries = 0;
  int i;
  for (i = 0; i < n; ++i) {
    h->data[i].key = NULL;
    h->data[i].del = 0;
  }

  return h;
}

static void quadratic_rehash(quadratic_hash_t *h) {
  int new_size = h->buckets * 15 / 10;
  int i;

  for (i = 0; SIZES[i] != 0; ++i) {
    if (SIZES[i] > new_size) {
      new_size = SIZES[i];
      break;
    }
  }

  debug("Resize to %d\n", new_size);
  quadratic_hash_t *h2 = quadratic_new0(new_size);

  for (i = 0; i < h->buckets; ++i) {
    if (h->data[i].key != NULL) {
      quadratic_add0(h2, h->data[i].key);
    }
  }

  free(h->data);
  h->data = h2->data;
  h->buckets = h2->buckets;
  free(h2);
}

