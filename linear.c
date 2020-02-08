#include <string.h>
#include <stdlib.h>

#include "linear.h"

#include "strhash.h"
#include "new.h"
#include "key_list.h"
#include "debug.h"

static const int MAX_LOAD = 70;
static const int INIT_SIZE = 10;

struct _bucket_t {
  char *key;
  int del;
};

struct _linear_hash_t {
  int entries;
  int buckets;
  bucket_t *data;
};

static linear_hash_t * linear_new0(int n);
static void linear_rehash(linear_hash_t *h);

linear_hash_t * linear_new() {
  return linear_new0(INIT_SIZE);
}

void linear_free(linear_hash_t *h) {
  int i;
  for (i=0; i<h->buckets; ++i) free(h->data[i].key);
  free(h->data);
  free(h);
}

static void linear_add0(linear_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets;
  while (h->data[v].key != NULL) v = (v + 1) % h->buckets;
  h->data[v].key = k;
  h->data[v].del = 0;
  ++ h->entries;
}

void linear_add(linear_hash_t *h, char *k) {
  if (linear_get(h, k) == 1) return;
  if (h->entries * 100 > h->buckets * MAX_LOAD) {
    linear_rehash(h);
  }
  linear_add0(h, strdup(k));
}

int linear_del(linear_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets;
  int i, found = 0;
  for (i = 0; i < h->buckets; ++i) {
    if (h->data[v].key == NULL && !h->data[v].del) break;
    if (h->data[v].key != NULL && !strcmp(h->data[v].key, k)) {
      free(h->data[v].key);
      h->data[v].key = NULL;
      h->data[v].del = 1;
      -- h->entries;
      found = 1;
      break;
    }
    v = (v + 1) % h->buckets;
  }
  return found;
}

int linear_get(linear_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets;
  int i, found = 0;
  for (i = 0; i < h->buckets; ++i) {
    debug("i:%d v:%d key: %s k:%s\n", i, v, h->data[v].key, k);
    if (h->data[v].key == NULL && !h->data[v].del) break;
    if (h->data[v].key != NULL && !strcmp(h->data[v].key, k)) {
      found = 1;
      break;
    }
    v = (v + 1) % h->buckets;
  }
  return found;
}

int linear_size(linear_hash_t *h) {
  return h->entries;
}

key_list_t * linear_keys(linear_hash_t *h) {
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

static linear_hash_t * linear_new0(int n) {
  linear_hash_t *h = new0(linear_hash_t);
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

static void linear_rehash(linear_hash_t *h) {
  int new_size = h->buckets * 15 / 10;

  debug("Resize to %d\n", new_size);
  linear_hash_t *h2 = linear_new0(new_size);

  int i;
  for (i = 0; i < h->buckets; ++i) {
    if (h->data[i].key != NULL) {
      linear_add0(h2, h->data[i].key);
    }
  }

  debug("delete old hash\n");

  free(h->data);
  h->data = h2->data;
  h->buckets = h2->buckets;
  free(h2);
}

