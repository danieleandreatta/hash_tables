#include <string.h>
#include <stdlib.h>

#include "double.h"

#include "strhash.h"
#include "new.h"
#include "key_list.h"
#include "debug.h"

static const int MAX_LOAD = 70;
static const int INIT_SIZE = 11;

static const int SIZES[] = {10, 17, 25, 37, 59, 89, 137, 211, 317, 479, 719, 1087, 1637, 2459, 3691, 5557, 8353, 12539, 18839, 28277, 42433, 63649, 95479, 143239, 214867, 322319, 483481, 725273, 1087937, 0};

struct _bucket_t {
  char *key;
  int del;
};

struct _double_hash_t {
  int entries;
  int buckets;
  bucket_t *data;
};

static double_hash_t * double_new0(int n);
static void double_rehash(double_hash_t *h);

double_hash_t * double_new() {
  return double_new0(INIT_SIZE);
}

void double_free(double_hash_t *h) {
  int i;
  for (i=0; i<h->buckets; ++i) free(h->data[i].key);
  free(h->data);
  free(h);
}

static void double_add0(double_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets;
  unsigned u = 1 + (strhash2(k) % (h->buckets - 1));
  int i;
  for (i = 0; i < h->buckets; ++i) {
    unsigned t = (v + i * u) % h->buckets;
    if (h->data[t].key == NULL) {
      h->data[t].key = k;
      h->data[t].del = 0;
      ++ h->entries;
      break;
    }
  }
}

void double_add(double_hash_t *h, char *k) {
  if (double_get(h, k) == 1) return;
  if (h->entries * 100 > h->buckets * MAX_LOAD) {
    double_rehash(h);
  }
  double_add0(h, strdup(k));
}

int double_del(double_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets;
  unsigned u = 1 + (strhash2(k) % (h->buckets - 1));
  int i, found = 0;
  for (i = 0; i < h->buckets; ++i) {
    unsigned t = (v + i * u) % h->buckets;
    if (h->data[t].key == NULL && !h->data[t].del) break;
    if (h->data[t].key != NULL && !strcmp(h->data[t].key, k)) {
      free(h->data[t].key);
      h->data[t].key = NULL;
      h->data[t].del = 1;
      -- h->entries;
      found = 1;
      break;
    }
  }
  return found;
}

int double_get(double_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets;
  unsigned u = 1 + (strhash2(k) % (h->buckets - 1));
  int i, found = 0;
  for (i = 0; i < h->buckets; ++i) {
    unsigned t = (v + i * u) % h->buckets;
    debug("i:%d v:%d key: %s k:%s\n", i, v, h->data[t].key, k);
    if (h->data[t].key == NULL && !h->data[t].del) break;
    if (h->data[t].key != NULL && !strcmp(h->data[t].key, k)) {
      found = 1;
      break;
    }
  }
  return found;
}

int double_size(double_hash_t *h) {
  return h->entries;
}

key_list_t * double_keys(double_hash_t *h) {
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

static double_hash_t * double_new0(int n) {
  double_hash_t *h = new0(double_hash_t);
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

static void double_rehash(double_hash_t *h) {
  int new_size = h->buckets * 15 / 10;
  int i;

  for (i = 0; SIZES[i] != 0; ++i) {
    if (SIZES[i] > new_size) {
      new_size = SIZES[i];
      break;
    }
  }

  debug("Resize to %d\n", new_size);
  double_hash_t *h2 = double_new0(new_size);

  for (i = 0; i < h->buckets; ++i) {
    if (h->data[i].key != NULL) {
      double_add0(h2, h->data[i].key);
    }
  }

  debug("delete old hash\n");

  free(h->data);
  h->data = h2->data;
  h->buckets = h2->buckets;
  free(h2);
}

