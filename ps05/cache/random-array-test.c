#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../minunit.h"

#include "./random-array.gen.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_random_array() {
  srand(time(NULL));
  assert(random_array(rand() % 1000) != NULL);
  return 0;
}

static char *test_random_array2() {
  assert(random_array2(rand() % 1000, rand() % 1000) != NULL);

  int64_t *in_order = random_array2(100, 0);
  for (int64_t i = 0; i < 100; i++) {
    assert_int64_eq(in_order[i], i);
  }
  return 0;
}

static char *all_tests() {
  run_test(test_random_array);
  run_test(test_random_array2);
  return 0;
}

int main(void) { test_main(all_tests); }
