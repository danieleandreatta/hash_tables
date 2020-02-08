#include <string.h>
#include <stdlib.h>

#include "coalesced.h"

#include "strhash.h"
#include "new.h"
#include "key_list.h"
#include "debug.h"

static const int MAX_LOAD = 70;
static const int MIN_LOAD = 20;
static const int INIT_SIZE = 10;

struct _bucket_t {
  char *key;
  //int del;
  bucket_t *next;
};

struct _coalesced_hash_t {
  int entries;
  int buckets;
  bucket_t *data;
};

static coalesced_hash_t * coalesced_new0(int n);
static void coalesced_rehash(coalesced_hash_t *h);
static bucket_t * get0(coalesced_hash_t *h, char *k);

coalesced_hash_t * coalesced_new() {
  return coalesced_new0(INIT_SIZE);
}

void coalesced_free(coalesced_hash_t *h) {
  int i;
  for (i=0; i<h->buckets; ++i) free(h->data[i].key);
  free(h->data);
  free(h);
}

static void coalesced_add0(coalesced_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets, u;
  int i;
  bucket_t *it;

  if (h->data[v].key == NULL) {
    // head cell is empty. Fill it 
    h->data[v].key = k;
    //h->data[v].del = 0;
    ++ h->entries;
    return;
  }

  // scan the existing chain to find and reuse deleted nodes
  for (i = 0, it = h->data[v].next; it != NULL && i < h->buckets; it = it->next, ++ i) {
    if (it->key == NULL) {
      it->key = k;
      //it->del = 0;
      ++ h->entries;
      return;
    }
  }

  // we have to find a new empty bucket
  for (i = 0; i < h->buckets; ++ i) {
    u = (v + i) % h->buckets;
    if (h->data[u].key == NULL) {
      h->data[u].key = k;
      h->data[u].next = NULL;
      //h->data[u].del = 0;
      ++ h->entries;
      break;
    }
  }

  // add to chain. find last element
  for (it = &h->data[v]; it->next != NULL; it = it->next);
  it->next = &h->data[u];
}

void coalesced_add(coalesced_hash_t *h, char *k) {
  if (coalesced_get(h, k) == 1) return;
  if (h->entries * 100 > h->buckets * MAX_LOAD) coalesced_rehash(h);
  coalesced_add0(h, strdup(k));
}

int coalesced_del(coalesced_hash_t *h, char *k) {
  bucket_t *b = get0(h, k);
  if (b != NULL) {
    free(b->key);
    //b->del = 1;
    b->key = NULL;
    -- h->entries;
  }
  // if (h->buckets > INIT_SIZE && h->entries * 100 < h->buckets * MIN_LOAD) coalesced_rehash(h);
  return b != NULL;
}

int coalesced_get(coalesced_hash_t *h, char *k) {
  return get0(h, k) != NULL;
}

int coalesced_size(coalesced_hash_t *h) {
  return h->entries;
}

key_list_t * coalesced_keys(coalesced_hash_t *h) {
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

static coalesced_hash_t * coalesced_new0(int n) {
  coalesced_hash_t *h = new0(coalesced_hash_t);
  h->data = new1(bucket_t, n);
  h->buckets = n;
  h->entries = 0;
  int i;
  for (i = 0; i < n; ++i) {
    h->data[i].key = NULL;
    h->data[i].next = NULL;
    //h->data[i].del = 0;
  }

  return h;
}

static void coalesced_rehash(coalesced_hash_t *h) {
  int new_size = h->entries * 2;

  debug("Resize to %d\n", new_size);
  coalesced_hash_t *h2 = coalesced_new0(new_size);

  int i;
  for (i = 0; i < h->buckets; ++i) {
    if (h->data[i].key != NULL) {
      coalesced_add0(h2, h->data[i].key);
    }
  }

  debug("delete old hash\n");

  free(h->data);
  h->data = h2->data;
  h->buckets = h2->buckets;
  free(h2);
}

static bucket_t * get0(coalesced_hash_t *h, char *k) {
  unsigned v = strhash(k) % h->buckets;
  bucket_t *it = NULL;
  int i;
  for (i = 0, it = &h->data[v]; it != NULL && i < h->buckets; it = it->next, ++ i) {
    debug("v: %d key: %s\n", v, it->key);
    if (it->key != NULL && !strcmp(it->key, k)) break;
  }
  return it;
}

