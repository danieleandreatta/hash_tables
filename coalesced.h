#include "key_list.h"

typedef struct _bucket_t bucket_t;

typedef struct _coalesced_hash_t coalesced_hash_t;

coalesced_hash_t * coalesced_new();
void coalesced_free(coalesced_hash_t *h);
void coalesced_add(coalesced_hash_t *h, char *key);
int coalesced_del(coalesced_hash_t *h, char *key);
int coalesced_get(coalesced_hash_t *h, char *key);
int coalesced_size(coalesced_hash_t *h);
key_list_t *coalesced_keys(coalesced_hash_t *h);

