#include "new.h"
#include "list.h"

typedef struct _entry_t entry_t;

struct _entry_t {
  entry_t *next;
  void *data;
}

struct _list_t {
  entry_t *head;
}

list_t * list_new(void) {
  list_t *l = new0(list_t);
  l->head = NULL;
}

list_t * list_add(list_t *l, void *d) {
  entry_t e = new0(entry_t);
  e->data = d;
}
