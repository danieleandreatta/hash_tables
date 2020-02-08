#include <string.h>

#include "chain.h"
#include "strhash.h"
#include "new.h"
#include "key_list.h"
#include "debug.h"

static const int MAX_LOAD = 10;
static const int INIT_SIZE = 10;

struct _bucket_t {
  char *key;
  bucket_t *next;
};

struct _chain_hash_t {
  int entries;
  int buckets;
  bucket_t **data;
};

static chain_hash_t * chain_new0(int n);
static void chain_rehash(chain_hash_t *h);
static void chain_alloc(chain_hash_t *h, int n);
static void chain_free_data(chain_hash_t *h);

chain_hash_t * chain_new() {
  return chain_new0(INIT_SIZE);
}

static chain_hash_t * chain_new0(int n) {
  chain_hash_t *h = new0(chain_hash_t);
  h->entries = 0;
  chain_alloc(h, n);
  return h;
}

void chain_free(chain_hash_t *h) {
  chain_free_data(h);
  free(h->data);
  free(h);
}

void add0(chain_hash_t *h, bucket_t *b) {
  unsigned v = strhash(b->key) % h->buckets;
  b->next = h->data[v];
  h->data[v] = b;
  ++ h->entries;
}

void chain_add(chain_hash_t *h, char *k) {
  if (chain_get(h, k) == 1) return;
  if (h->entries > h->buckets * MAX_LOAD) {
    debug("Rehash on key: %s\n", k);
    debug("Table size %d/%d -> ", h->buckets, h->entries);
    chain_rehash(h);
    debug("%d/%d\n", h->buckets, h->entries);
  }
  bucket_t *b = new0(bucket_t);
  b->key = strdup(k);
  add0(h, b);
}

int chain_del(chain_hash_t *h, char *key) {
  unsigned v = strhash(key) % h->buckets;
  bucket_t *b, *p;
  int found = 0;
  if (h->data[v] != NULL) {
    if (!strcmp(h->data[v]->key, key)) {
      b = h->data[v]->next;
      free(h->data[v]);
      h->data[v] = b;
      found = 1;
    } else {
      for(p = h->data[v], b = h->data[v]->next; b != NULL; p = p->next, b = b->next) {
        if (!strcmp(b->key, key)) {
          p->next = b->next;
          free(b);
          found = 1;
          break;
        }
      }
    }
    -- h->entries;
  }
  return found;
}

int chain_get(chain_hash_t *h, char *key) {
  unsigned v = strhash(key) % h->buckets;
  bucket_t *b;
  int found = 0;
  for(b = h->data[v]; b != NULL; b = b->next) {
    if (!strcmp(b->key, key)) {
      found = 1;
      break;
    }
  }

  return found;
}

int chain_size(chain_hash_t *h) {
  return h->entries;
}

key_list_t *chain_keys(chain_hash_t *h) {
  int i, j = 0;
  key_list_t *kl = keys_new(h->entries);
  for(i = 0; i < h->buckets; ++i) {
    bucket_t *b;
    for (b = h->data[i]; b != NULL; b = b->next) {
      keys_set(kl, j, b->key);
      ++ j;
    }
  }
  return kl;
}

static void chain_rehash(chain_hash_t *h) {
  int new_size = h->buckets * 15 / 10;
  chain_hash_t *h2 = chain_new0(new_size);
  bucket_t *a, *b;
  int i;

  for (i = 0; i < h->buckets; ++i) {
    if (h->data[i] != NULL) {
      a = h->data[i];
      do {
        b = a->next; 
        a->next = NULL;
        add0(h2, a);
        a = b;
      } while (a != NULL);
    }
  }
  free(h->data);
  h->data = h2->data;
  h->buckets = h2->buckets;
  free(h2);
}

static void chain_alloc(chain_hash_t *h, int n) {
  int i;
  h->buckets = n;
  h->data = new1(bucket_t*, n);
  for (i = 0; i < n; ++i) {
    h->data[i] = NULL;
  }
}

static void chain_free_data(chain_hash_t *h) {
  int i;
  bucket_t *b, *c;
  for (i = 0; i < h->buckets; ++ i) {
    for (b = h->data[i]; b != NULL;) {
      c = b->next;
      free(b);
      b = c;
    }
  }
}
