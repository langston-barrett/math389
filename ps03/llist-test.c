#include <stdio.h>
#include <stdint.h>
#include "../minunit.h"
#include "llist.gen.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_new_llist() {
  llist *l = new_llist();
  assert(l->first == NULL);
  free(l);
  return 0;
}

static char *test_llist_is_empty() {
  assert(true == llist_is_empty(new_llist()));
  return 0;
}

static char *test_llist_insert_length() {
  llist *l = new_llist();
  for (int i = 0; i < 1000; i++) {
    // construct error message
    char *msg = malloc(30*sizeof(char));
    sprintf(msg, "length(llist) != %d", i);

    // construct key
    char *key = malloc(10*sizeof(char));
    sprintf(key, "key%d", i);

    assert(i == llist_length(l));

    // insert k/v
    llist_insert(l, key, i);
  }
  free(l);
  return 0;
}

static char *test_llist_lookup() {
  llist *l = new_llist();
  assert(NULL == llist_lookup(l, "key"));
  llist_insert(l, "key", 1);
  assert(NULL != llist_lookup(l, "key"));
  free(l);
  return 0;
}

static char *all_tests() {
  run_test(test_new_llist);
  run_test(test_llist_is_empty);
  run_test(test_llist_insert_length);
  run_test(test_llist_lookup);
  return 0;
}

int main(void) { test_main(all_tests); }
