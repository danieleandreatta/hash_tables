#include <string.h>

#include "choice2.h"
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

typedef struct _head_t {
  int n;
  bucket_t *b;
} head_t;

struct _choice2_hash_t {
  int entries;
  int buckets;
  head_t *data;
};

static choice2_hash_t * choice2_new0(int n);
static void choice2_rehash(choice2_hash_t *h);
static void choice2_alloc(choice2_hash_t *h, int n);
static void choice2_free_data(choice2_hash_t *h);

choice2_hash_t * choice2_new() {
  return choice2_new0(INIT_SIZE);
}

static choice2_hash_t * choice2_new0(int n) {
  choice2_hash_t *h = new0(choice2_hash_t);
  h->entries = 0;
  choice2_alloc(h, n);
  return h;
}

void choice2_free(choice2_hash_t *h) {
  choice2_free_data(h);
  free(h->data);
  free(h);
}

static void add0(choice2_hash_t *h, bucket_t *b) {
  unsigned u = strhash(b->key) % h->buckets;
  unsigned v = strhash2(b->key) % h->buckets;
  unsigned z = h->data[u].n > h->data[v].n ? v : u; 
  b->next = h->data[z].b;
  h->data[z].b = b;
  ++ h->data[z].n;
  ++ h->entries;
}

void choice2_add(choice2_hash_t *h, char *k) {
  if (choice2_get(h, k) == 1) return;
  if (h->entries > h->buckets * MAX_LOAD) {
    debug("Rehash on key: %s\n", k);
    debug("Table size %d/%d -> ", h->buckets, h->entries);
    choice2_rehash(h);
    debug("%d/%d\n", h->buckets, h->entries);
  }
  bucket_t *b = new0(bucket_t);
  b->key = strdup(k);
  add0(h, b);
}

static int try_del0(choice2_hash_t *h, char *key, unsigned v) {
  bucket_t *b, *p;
  int found = 0;
  if (h->data[v].b != NULL) {
    if (!strcmp(h->data[v].b->key, key)) {
      b = h->data[v].b->next;
      free(h->data[v].b);
      h->data[v].b = b;
      -- h->data[v].n;
      found = 1;
      -- h->entries;
    } else {
      for(p = h->data[v].b, b = h->data[v].b->next; b != NULL; p = p->next, b = b->next) {
        if (!strcmp(b->key, key)) {
          p->next = b->next;
          free(b);
          found = 1;
          -- h->data[v].n;
          -- h->entries;
          break;
        }
      }
    }
  }
  return found;
}

int choice2_del(choice2_hash_t *h, char *key) {
  unsigned v = strhash(key) % h->buckets;
  if (try_del0(h, key, v)) return 1;
  v = strhash2(key) % h->buckets;
  if (try_del0(h, key, v)) return 1;
  return 0;
}

static bucket_t * get0(choice2_hash_t *h, char *key) {
  bucket_t *b;

  unsigned u = strhash(key) % h->buckets;
  for(b = h->data[u].b; b != NULL; b = b->next) {
    if (!strcmp(b->key, key)) {
      return b;
    }
  }

  unsigned v = strhash2(key) % h->buckets;
  for(b = h->data[v].b; b != NULL; b = b->next) {
    if (!strcmp(b->key, key)) {
      return b;
    }
  }

  return NULL;
}

int choice2_get(choice2_hash_t *h, char *key) {
  return get0(h, key) != NULL;
}

int choice2_size(choice2_hash_t *h) {
  return h->entries;
}

key_list_t *choice2_keys(choice2_hash_t *h) {
  int i, j = 0;
  key_list_t *kl = keys_new(h->entries);
  for(i = 0; i < h->buckets; ++i) {
    bucket_t *b;
    for (b = h->data[i].b; b != NULL; b = b->next) {
      keys_set(kl, j, b->key);
      ++ j;
    }
  }
  return kl;
}

static void choice2_rehash(choice2_hash_t *h) {
  int new_size = h->buckets * 15 / 10;
  choice2_hash_t *h2 = choice2_new0(new_size);
  bucket_t *a, *b;
  int i;

  for (i = 0; i < h->buckets; ++i) {
    if (h->data[i].b != NULL) {
      a = h->data[i].b;
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

static void choice2_alloc(choice2_hash_t *h, int n) {
  int i;
  h->buckets = n;
  h->data = new1(head_t, n);
  for (i = 0; i < n; ++i) {
    h->data[i].b = NULL;
    h->data[i].n = 0;
  }
}

static void choice2_free_data(choice2_hash_t *h) {
  int i;
  bucket_t *b, *c;
  for (i = 0; i < h->buckets; ++ i) {
    for (b = h->data[i].b; b != NULL;) {
      c = b->next;
      free(b);
      b = c;
    }
  }
}
