#include "key_list.h"

typedef struct _bucket_t bucket_t;

typedef struct _choice2_hash_t choice2_hash_t;

choice2_hash_t * choice2_new();
void choice2_free(choice2_hash_t *h);
void choice2_add(choice2_hash_t *h, char *key);
int choice2_del(choice2_hash_t *h, char *key);
int choice2_get(choice2_hash_t *h, char *key);
int choice2_size(choice2_hash_t *h);
key_list_t *choice2_keys(choice2_hash_t *h);
