#ifndef __KEY_LIST__
#define __KEY_LIST__
typedef struct _key_list_t key_list_t;

int keys_size(key_list_t *l);
char *keys_get(key_list_t *l, int n);
key_list_t *keys_new(int n);
void keys_set(key_list_t *l, int n, char *k);
void keys_free(key_list_t *l);
#endif
