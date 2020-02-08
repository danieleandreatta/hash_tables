#include "key_list.h"

typedef struct _bucket_t bucket_t;

typedef struct _double_hash_t double_hash_t;

double_hash_t * double_new();
void double_free(double_hash_t *h);
void double_add(double_hash_t *h, char *key);
int double_del(double_hash_t *h, char *key);
int double_get(double_hash_t *h, char *key);
int double_size(double_hash_t *h);
key_list_t *double_keys(double_hash_t *h);

