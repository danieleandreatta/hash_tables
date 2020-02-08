#include "key_list.h"

typedef struct _bucket_t bucket_t;

typedef struct _hopscotch_hash_t hopscotch_hash_t;

hopscotch_hash_t * hopscotch_new();
void hopscotch_free(hopscotch_hash_t *h);
void hopscotch_add(hopscotch_hash_t *h, char *key);
int hopscotch_del(hopscotch_hash_t *h, char *key);
int hopscotch_get(hopscotch_hash_t *h, char *key);
int hopscotch_size(hopscotch_hash_t *h);
key_list_t *hopscotch_keys(hopscotch_hash_t *h);

