#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../key_list.h"
#include "../chain.h"

int main(void) {
  key_list_t *keys;
  int i, j;
  char xxx[] = "\0\0\0";

  chain_hash_t *h = chain_new();
  
  printf("%d\n", __LINE__);
  chain_add(h, "test");

  printf("%d\n", __LINE__);
  assert(chain_get(h, "test") == 1);
  printf("%d\n", __LINE__);
  assert(chain_get(h, "blah") == 0);
  printf("%d\n", __LINE__);
  assert(chain_size(h) == 1);

  printf("%d\n", __LINE__);
  keys = chain_keys(h);
  printf("%d\n", __LINE__);
  printf("num keys: %d\n", keys_size(keys));
  assert(keys_size(keys) == 1);
  printf("%d\n", __LINE__);
  assert(strcmp(keys_get(keys, 0), "test") == 0);

  printf("%d\n", __LINE__);
  chain_del(h, "test");
  printf("%d\n", __LINE__);
  assert(chain_size(h) == 0);
  printf("%d\n", __LINE__);
  assert(chain_get(h, "test") == 0);

  // Add 676 words
  for (i = 'a'; i <= 'z'; ++i) {
    xxx[0] = i;
    for (j = 'a'; j <= 'z'; ++j) {
      xxx[1] = j;
      chain_add(h, xxx);
    }
  }
      
  printf("%d\n", __LINE__);
  assert(chain_size(h) == 676);

  // Delete 676 words
  for (i = 'a'; i <= 'z'; ++i) {
    xxx[0] = i;
    for (j = 'a'; j <= 'z'; ++j) {
      xxx[1] = j;
      chain_del(h, xxx);
    }
  }

  printf("%d\n", __LINE__);
  assert(chain_size(h) == 0);
      
  printf("%d\n", __LINE__);
  chain_free(h);
  return 0;
}
