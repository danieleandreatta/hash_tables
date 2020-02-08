#include <string.h>
#include <stdlib.h>

#include "cuckoo.h"

#include "strhash.h"
#include "new.h"
#include "key_list.h"
#include "debug.h"

static const int MAX_LOAD = 70;
static const int INIT_SIZE = 10;

typedef unsigned (*HashFunc)(char*);
static const HashFunc hash[] = {strhash, strhash2, strhash3, strhash4};
static const int hashes = (sizeof(hash)/sizeof(HashFunc));

struct _bucket_t {
  char *key;
};

struct _cuckoo_hash_t {
  int entries;
  int buckets;
  bucket_t *data;
};

static bucket_t * get0(cuckoo_hash_t *h, char *key);
static cuckoo_hash_t * cuckoo_new0(int n);
static void cuckoo_rehash(cuckoo_hash_t *h);

cuckoo_hash_t * cuckoo_new() {
  return cuckoo_new0(INIT_SIZE);
}

void cuckoo_free(cuckoo_hash_t *h) {
  int i;
  for (i=0; i<h->buckets; ++i) free(h->data[i].key);
  free(h->data);
  free(h);
}

static void cuckoo_add0(cuckoo_hash_t *h, char *k) {
  char *key = k, *t;
  unsigned v;
  int i;
  
  ++ h->entries;

  while (key != NULL) {
    for (i = 0; i < hashes; ++i) {
      v = hash[i](key) % h->buckets;
      //printf("%d %s %d\n", i, key, v);
      if (h->data[v].key == NULL) {
        h->data[v].key = key;
        return;
      }
    }

    // evict one key at random
    v = hash[rand() % hashes](key) % h->buckets;
    t = h->data[v].key;
    h->data[v].key = key;
    key = t;
  }

}

void cuckoo_add(cuckoo_hash_t *h, char *k) {
  if (cuckoo_get(h, k) == 1) return;
  if (h->entries * 100 > h->buckets * MAX_LOAD) {
    cuckoo_rehash(h);
  }
  cuckoo_add0(h, strdup(k));
}

int cuckoo_del(cuckoo_hash_t *h, char *k) {
  bucket_t *b = get0(h, k);

  if (b == NULL) return 0;

  free(b->key);
  b->key = NULL;
  -- h->entries;

  return 1;
}

int cuckoo_get(cuckoo_hash_t *h, char *k) {
  return get0(h, k) != NULL;
}

int cuckoo_size(cuckoo_hash_t *h) {
  return h->entries;
}

key_list_t * cuckoo_keys(cuckoo_hash_t *h) {
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

static bucket_t * get0(cuckoo_hash_t *h, char *key) {
  unsigned v;
  int i;

  for (i = 0; i < hashes; ++i) {
    v = hash[i](key) % h->buckets;
    if (h->data[v].key != NULL && !strcmp(key, h->data[v].key)) {
      return &h->data[v];
    }
  }

  return NULL; 
}

static cuckoo_hash_t * cuckoo_new0(int n) {
  cuckoo_hash_t *h = new0(cuckoo_hash_t);
  h->data = new1(bucket_t, n);
  h->buckets = n;
  h->entries = 0;
  int i;
  for (i = 0; i < n; ++i) {
    h->data[i].key = NULL;
  }

  return h;
}

static void cuckoo_rehash(cuckoo_hash_t *h) {
  int new_size = h->buckets * 15 / 10;

  debug("Resize to %d\n", new_size);
  cuckoo_hash_t *h2 = cuckoo_new0(new_size);

  int i;
  for (i = 0; i < h->buckets; ++i) {
    if (h->data[i].key != NULL) {
      cuckoo_add0(h2, h->data[i].key);
    }
  }

  debug("delete old hash\n");

  free(h->data);
  h->data = h2->data;
  h->buckets = h2->buckets;
  free(h2);
}

