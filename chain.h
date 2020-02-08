#include "key_list.h"

typedef struct _bucket_t bucket_t;

typedef struct _chain_hash_t chain_hash_t;

chain_hash_t * chain_new();
void chain_free(chain_hash_t *h);
void chain_add(chain_hash_t *h, char *key);
int chain_del(chain_hash_t *h, char *key);
int chain_get(chain_hash_t *h, char *key);
int chain_size(chain_hash_t *h);
key_list_t *chain_keys(chain_hash_t *h);
