#include <stdio.h>
#include "../minunit.h"
#include "hash-table.gen.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_new_dict() {
  for (int i = 0; i < 1000; i++) {
    dict *d = new_dict(i);
    assert(d != NULL);
    free(d);
  }
  return 0;
}

static char *test_hash() {
  char *str = "string";
  assert(hash("") == 0);
  assert(hash("key") == 329);
  assert(hash(str) == 663);
  return 0;
}

static char *test_get_bucket() {
  dict *d = new_dict(30);
  assert(NULL != get_bucket(d, "key"));
  return 0;
}

static char *test_insert_lookup() {
  dict *d = new_dict(10);
  insert(d, "key", 0);
  insert(d, "key2", (int64_t) 1);
  insert(d, "test", (int64_t) 2);
  insert(d, "value", (int64_t) 3);

  assert(look_up(d, "key") == 0);
  assert(look_up(d, "key2") == 1);
  assert(look_up(d, "test") == 2);
  assert(look_up(d, "value") == 3);
  return 0;
}

static char *test_update() {
  dict *d = new_dict(10);
  insert(d, "key", 0);
  assert(look_up(d, "key") == 0);
  update(d, "key", 13);
  assert(look_up(d, "key") == 13);
  update(d, "key", 23);
  assert(look_up(d, "key") == 23);
  return 0;
}

static char *test_remove() {
  dict *d = new_dict(10);
  insert(d, "key", 0);
  assert(look_up(d, "key") == 0);
  dict_remove(d, "key");
  assert(look_up(d, "key") == -1);
  return 0;
}

static char *all_tests() {
  run_test(test_new_dict);
  run_test(test_hash);
  run_test(test_get_bucket);
  run_test(test_insert_lookup);
  run_test(test_update);
  run_test(test_remove);
  return 0;
}

int main(void) { test_main(all_tests); }
