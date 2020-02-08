#include "key_list.h"

typedef struct _bucket_t bucket_t;

typedef struct _linear_hash_t linear_hash_t;

linear_hash_t * linear_new();
void linear_free(linear_hash_t *h);
void linear_add(linear_hash_t *h, char *key);
int linear_del(linear_hash_t *h, char *key);
int linear_get(linear_hash_t *h, char *key);
int linear_size(linear_hash_t *h);
key_list_t *linear_keys(linear_hash_t *h);

