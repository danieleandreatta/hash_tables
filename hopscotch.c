#include <string.h>
#include <stdlib.h>

#include "hopscotch.h"

#include "strhash.h"
#include "new.h"
#include "key_list.h"
#include "debug.h"

static const int MAX_LOAD = 70;
static const int INIT_SIZE = 10;
static const int MAX_DIST = 32;

struct _bucket_t {
  char *key;
};

struct _hopscotch_hash_t {
  int entries;
  int buckets;
  bucket_t *data;
};

static hopscotch_hash_t * hopscotch_new0(int n);
static void hopscotch_rehash(hopscotch_hash_t *h);
static bucket_t * get0(hopscotch_hash_t *h, char *k);
static int min(int a, int b);

hopscotch_hash_t * hopscotch_new() {
  return hopscotch_new0(INIT_SIZE);
}

void hopscotch_free(hopscotch_hash_t *h) {
  int i;
  for (i=0; i<h->buckets; ++i) free(h->data[i].key);
  free(h->data);
  free(h);
}

static void add0(hopscotch_hash_t *h, char *k) {
  int v = strhash(k) % h->buckets;
  int u = v;
  ++ h->entries;
  while (h->data[u].key != NULL) u = (u + 1) % h->buckets;
  if ((u - v) % h->buckets < MAX_DIST) {
    h->data[u].key = k;
    return;
  }
  // outside neighbourhood
  int i, z, found;
  while ((u - v) % h->buckets >= MAX_DIST) {
    found = 0;
    for (i = (u - MAX_DIST) % h->buckets; (u - i) % h->buckets != 0; i = (i + 1) % h->buckets) {
      z = strhash(h->data[i].key) % h->buckets;
      if ((u - z) % h->buckets < MAX_DIST) {
        h->data[u].key = h->data[i].key;
        u = i;
        found = 1;
        break;
      }
    }
    if (!found) {
      h->data[u].key = k;
      hopscotch_rehash(h);
      return;
    }
  }
  h->data[u].key = k;
}

void hopscotch_add(hopscotch_hash_t *h, char *k) {
  if (hopscotch_get(h, k) == 1) return;
  if (h->entries * 100 > h->buckets * MAX_LOAD) {
    hopscotch_rehash(h);
  }
  add0(h, strdup(k));
}

int hopscotch_del(hopscotch_hash_t *h, char *k) {
  bucket_t *b = get0(h, k);
  if (b != NULL) {
    free(b->key);
    b->key = NULL;
    -- h->entries;
  }
  return b != NULL;
}

int hopscotch_get(hopscotch_hash_t *h, char *k) {
  return get0(h, k) != NULL;
}

int hopscotch_size(hopscotch_hash_t *h) {
  return h->entries;
}

key_list_t * hopscotch_keys(hopscotch_hash_t *h) {
  int i, j = 0;
  key_list_t *kl = keys_new(h->entries);
  for(i = 0; i < h->buckets; ++i) {
    if (h->data[i].key != NULL) {
      keys_set(kl, j, h->data[i].key);
      ++ j;
    }
  }
  return kl;
}

static bucket_t * get0(hopscotch_hash_t *h, char *k) {
  int v = strhash(k) % h->buckets, u;
  int i;
  bucket_t *b = NULL;
  for (i = 0; i < min(MAX_DIST, h->buckets); ++i) {
    u = (v + i) % h->buckets;
    if (h->data[u].key != NULL && !strcmp(h->data[u].key, k)) {
      b = &h->data[u];
      break;
    }
  }
  return b;
}

static hopscotch_hash_t * hopscotch_new0(int n) {
  hopscotch_hash_t *h = new0(hopscotch_hash_t);
  h->data = new1(bucket_t, n);
  h->buckets = n;
  h->entries = 0;
  int i;
  for (i = 0; i < n; ++i) {
    h->data[i].key = NULL;
  }

  return h;
}

static void hopscotch_rehash(hopscotch_hash_t *h) {
  int new_size = h->buckets * 15 / 10;

  debug("Resize to %d\n", new_size);
  hopscotch_hash_t *h2 = hopscotch_new0(new_size);

  int i;
  for (i = 0; i < h->buckets; ++i) {
    if (h->data[i].key != NULL) {
      add0(h2, h->data[i].key);
    }
  }

  debug("delete old hash\n");

  free(h->data);
  h->data = h2->data;
  h->buckets = h2->buckets;
  free(h2);
}

static int min(int a, int b) {
  return a < b ? a : b;
}
