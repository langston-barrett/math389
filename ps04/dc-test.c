#include <stdio.h>
#include <stdint.h>
#include "../minunit.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

extern int64_t mod(int64_t dividend, int64_t divisor);

static char *test_mod() {
  assert_int64_eq(1,  mod(10, 3));
  assert_int64_eq(2,  mod(10, 4));
  assert_int64_eq(0,  mod(10, 5));
  assert_int64_eq(0,  mod(10, 5));
  assert_int64_eq(1,  mod(9, 2));
  assert_int64_eq(1,  mod(-3, 2));
  assert_int64_eq(0,  mod(-10, 2));
  assert_int64_eq(1,  mod(-11, 2));
  assert_int64_eq(1, mod(-1, 8));
  return 0;
}

static char *all_tests() {
  run_test(test_mod);
  return 0;
}

int main(void) { test_main(all_tests); }
