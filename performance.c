#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "chain.h"
#include "choice2.h"
#include "linear.h"
#include "quadratic.h"
#include "double.h"
#include "coalesced.h"
#include "cuckoo.h"
#include "hopscotch.h"
#include "key_list.h"

typedef void* (*NewFunc)(void);
typedef void* (*AddFunc)(void*, char*);
typedef int (*GetFunc)(void*, char*);
typedef int (*DelFunc)(void*, char*);
typedef int (*SizeFunc)(void*);
typedef key_list_t* (*KeysFunc)(void*);

const int num_words = 99154;

int times_ms(clock_t t0, clock_t t1) {
  return 1000 * (t1-t0) / CLOCKS_PER_SEC;
}

void perf_test1(char *name, char **words, NewFunc new, AddFunc add, GetFunc get, DelFunc del, SizeFunc size, KeysFunc keys) {
  int i;
  // chaining hash table
  void *h0 = new();
  clock_t t0 = clock();
  for (i = 0; i < num_words; ++i) {
    add(h0, words[i]);
  }
  //puts("done add");
  assert(size(h0) == num_words);
  clock_t t1 = clock();
  for (i = 0; i < num_words; ++i) {
    int x = get(h0, words[i]);
    if (x == 0) printf("Key not found: %s\n", words[i]);
  }
  //puts("done get");
  clock_t t2 = clock();
  for (i = 0; i < num_words; ++i) {
    if (del(h0, words[i]) == 0) {
      printf("Deleting: key not found: %s\n", words[i]);
    }
  }
  //puts("done del");
  if (size(h0) != 0) {
    printf("hash table not empty: %d\n", size(h0));
    key_list_t *kl = keys(h0);
    int j;
    for (j = 0; j < keys_size(kl); ++j) {
      printf("%5d: %s\n", j, keys_get(kl, j));
    }
  }
  clock_t t3 = clock();
  for (i = 0; i < num_words; ++i) {
    add(h0, words[i]);
  }
  ///puts("done re-add");
  assert(size(h0) == num_words);
  clock_t t4 = clock();
  srand(123);
  for (i = 0; i < 300000; ++i) {
    int j = rand() % num_words;
    //nprintf("iteration %d word %d %s\n", i, j, words[j]);
    if (rand() % 2) {
      add(h0, words[j]);
    } else {
      del(h0, words[j]);
    }
  }
  clock_t t5 = clock();

  printf("%20s | %6d | %6d | %6d | %6d | %6d\n", name, times_ms(t0, t1), times_ms(t1, t2), times_ms(t2, t3), times_ms(t3, t4), times_ms(t4, t5));
}
void perf_test(char *name, char **words, NewFunc new, AddFunc add, GetFunc get, DelFunc del, SizeFunc size, KeysFunc keys) {
  int i;
  // chaining hash table
  void *h0 = new();
  clock_t t0 = clock();
  for (i = 0; i < num_words; ++i) {
    add(h0, words[i]);
  }
  assert(size(h0) == num_words);
  clock_t t1 = clock();
  for (i = 0; i < num_words; ++i) {
    int x = get(h0, words[i]);
    if (x == 0) printf("Key not found: %s\n", words[i]);
  }
  clock_t t2 = clock();
  for (i = 0; i < num_words; ++i) {
    if (del(h0, words[i]) == 0) {
      printf("Deleting: key not found: %s\n", words[i]);
    }
  }
  if (size(h0) != 0) {
    printf("hash table not empty: %d\n", size(h0));
    key_list_t *kl = keys(h0);
    int j;
    for (j = 0; j < keys_size(kl); ++j) {
      printf("%5d: %s\n", j, keys_get(kl, j));
    }
  }
  clock_t t3 = clock();
  for (i = 0; i < num_words; ++i) {
    add(h0, words[i]);
  }
  assert(size(h0) == num_words);
  clock_t t4 = clock();
  srand(123);
  for (i = 0; i < 300000; ++i) {
    int j = rand() % num_words;
    if (rand() % 2) {
      add(h0, words[j]);
    } else {
      del(h0, words[j]);
    }
  }
  clock_t t5 = clock();

  printf("%20s | %6d | %6d | %6d | %6d | %6d\n", name, times_ms(t0, t1), times_ms(t1, t2), times_ms(t2, t3), times_ms(t3, t4), times_ms(t4, t5));
}

int main(void) {
  FILE *f = fopen("words", "r");
  char **words = calloc(num_words, sizeof(char*));
  int i = 0;
  size_t n;
  for (i=0; i<num_words; ++i) words[i] = NULL;
  for (i = 0; !feof(f); ++i) {
    getline(&words[i], &n, f);
    words[i][strlen(words[i])-1] = '\0';
    // printf("%d %s\n", i, words[i]);
  }
  printf("%20s |    add |    get |    del | re-add | random\n", "type \\ operation");
  perf_test("chaining", words,
            (NewFunc)chain_new, (AddFunc)chain_add, (GetFunc)chain_get,
            (DelFunc)chain_del, (SizeFunc)chain_size, (KeysFunc)chain_keys);
  perf_test("2choice", words,
            (NewFunc)choice2_new, (AddFunc)choice2_add, (GetFunc)choice2_get,
            (DelFunc)choice2_del, (SizeFunc)choice2_size, (KeysFunc)choice2_keys);
  perf_test("linear", words, 
            (NewFunc)linear_new, (AddFunc)linear_add, (GetFunc)linear_get, 
            (DelFunc)linear_del, (SizeFunc)linear_size, (KeysFunc)linear_keys);
  perf_test("quadratic", words, 
            (NewFunc)quadratic_new, (AddFunc)quadratic_add, (GetFunc)quadratic_get,
            (DelFunc)quadratic_del, (SizeFunc)quadratic_size, (KeysFunc)quadratic_keys);
  perf_test("double hash", words,
            (NewFunc)double_new, (AddFunc)double_add, (GetFunc)double_get,
            (DelFunc)double_del, (SizeFunc)double_size, (KeysFunc)double_keys);
  perf_test("coalesced", words,
            (NewFunc)coalesced_new, (AddFunc)coalesced_add, (GetFunc)coalesced_get,
            (DelFunc)coalesced_del, (SizeFunc)coalesced_size, (KeysFunc)coalesced_keys);
  perf_test("cuckoo", words,
            (NewFunc)cuckoo_new, (AddFunc)cuckoo_add, (GetFunc)cuckoo_get,
            (DelFunc)cuckoo_del, (SizeFunc)cuckoo_size, (KeysFunc)cuckoo_keys);
  perf_test1("hopscotch", words,
            (NewFunc)hopscotch_new, (AddFunc)hopscotch_add, (GetFunc)hopscotch_get,
            (DelFunc)hopscotch_del, (SizeFunc)hopscotch_size, (KeysFunc)hopscotch_keys);
  return 0;
}

