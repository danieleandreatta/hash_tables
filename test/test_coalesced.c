#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../key_list.h"
#include "../coalesced.h"

int main(void) {
  key_list_t *keys;
  int i, j;
  char xxx[] = "\0\0\0";

  coalesced_hash_t *h = coalesced_new();
  
  printf("%d\n", __LINE__);
  coalesced_add(h, "test");

  printf("%d\n", __LINE__);
  assert(coalesced_get(h, "test") == 1);
  printf("%d\n", __LINE__);
  assert(coalesced_get(h, "blah") == 0);
  printf("%d\n", __LINE__);
  assert(coalesced_size(h) == 1);

  printf("%d\n", __LINE__);
  keys = coalesced_keys(h);
  printf("%d\n", __LINE__);
  printf("num keys: %d\n", keys_size(keys));
  assert(keys_size(keys) == 1);
  printf("%d\n", __LINE__);
  assert(strcmp(keys_get(keys, 0), "test") == 0);

  printf("%d\n", __LINE__);
  coalesced_del(h, "test");
  printf("%d\n", __LINE__);
  assert(coalesced_size(h) == 0);
  printf("%d\n", __LINE__);
  assert(coalesced_get(h, "test") == 0);

  printf("%d\n", __LINE__);
  // Add 676 words
  for (i = 'a'; i <= 'z'; ++i) {
    xxx[0] = i;
    for (j = 'a'; j <= 'z'; ++j) {
      xxx[1] = j;
      coalesced_add(h, xxx);
    }
  }
      
  printf("%d\n", __LINE__);
  assert(coalesced_size(h) == 676);

  // Delete 676 words
  for (i = 'a'; i <= 'z'; ++i) {
    xxx[0] = i;
    for (j = 'a'; j <= 'z'; ++j) {
      xxx[1] = j;
      coalesced_del(h, xxx);
    }
  }

  printf("%d size: %d expected: %d\n", __LINE__, coalesced_size(h), 0);
  assert(coalesced_size(h) == 0);
      
  printf("%d\n", __LINE__);
  coalesced_free(h);
  return 0;
}
