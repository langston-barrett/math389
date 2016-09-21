#include "hash-table.h"

dict *new_dict(int64_t size) {
  dict *self = malloc(sizeof(dict));
  self->size = size;
  self->buckets = malloc(size*sizeof(llist));
  for (int64_t i = 0; i < size; i++) {
    self->buckets[i] = malloc(sizeof(llist));
  }
  return self;
}

// hash deterministically by summing the characters as int64_ts
int64_t hash(char *k) {
  int64_t sum = 0;
  for (uint64_t i = 0; i < strlen(k); i++) {
    sum = sum + (int64_t) k[i];
  }
  return sum;
}

llist *get_bucket(dict *d, char *k) { return d->buckets[hash(k) % d->size]; }

void insert(dict *d, char *k, int64_t v) {
  llist_insert(get_bucket(d, k), k, v);
}

// returns -1 when key not found
int64_t look_up(dict *d, char *k) {
  lnode *node = llist_lookup(get_bucket(d, k), k);
  if (node == NULL) return -1;
  return node->value;
}

int64_t update(dict *d, char *k, int64_t v) {
  lnode *node = llist_lookup(get_bucket(d, k), k);
  if (node == NULL) llist_insert(get_bucket(d, k), k, v);
  else node->value = v;
}

void dict_remove(dict *d, char *k) { llist_remove(get_bucket(d, k), k); }
