#include "key_list.h"

typedef struct _bucket_t bucket_t;

typedef struct _cuckoo_hash_t cuckoo_hash_t;

cuckoo_hash_t * cuckoo_new();
void cuckoo_free(cuckoo_hash_t *h);
void cuckoo_add(cuckoo_hash_t *h, char *key);
int cuckoo_del(cuckoo_hash_t *h, char *key);
int cuckoo_get(cuckoo_hash_t *h, char *key);
int cuckoo_size(cuckoo_hash_t *h);
key_list_t *cuckoo_keys(cuckoo_hash_t *h);

