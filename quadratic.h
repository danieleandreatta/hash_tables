#include "key_list.h"

typedef struct _bucket_t bucket_t;

typedef struct _quadratic_hash_t quadratic_hash_t;

quadratic_hash_t * quadratic_new();
void quadratic_free(quadratic_hash_t *h);
void quadratic_add(quadratic_hash_t *h, char *key);
int quadratic_del(quadratic_hash_t *h, char *key);
int quadratic_get(quadratic_hash_t *h, char *key);
int quadratic_size(quadratic_hash_t *h);
key_list_t *quadratic_keys(quadratic_hash_t *h);

