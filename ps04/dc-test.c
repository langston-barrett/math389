#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../minunit.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

extern int64_t mod(int64_t dividend, int64_t divisor);
extern int64_t divide(int64_t dividend, int64_t divisor);

static char *test_mod() {
  // preliminary hand-made tests
  assert_int64_eq(1,  mod(10, 3));
  assert_int64_eq(0,  mod(10, 5));
  assert_int64_eq(4,  mod(10, 6));
  assert_int64_eq(1,  mod(9, 2));
  assert_int64_eq(1,  mod(-3, 2));
  assert_int64_eq(0,  mod(-10, 2));
  assert_int64_eq(1,  mod(-10, 3));
  assert_int64_eq(1, mod(-1, 8));
  // automated tests - does it behave like C's mod?
  // note - C's mod handles negative values incorrectly.
  for (int64_t i = -1000; i < 1000; i++) {
    for (int64_t j = 1; j < 10; j++) {
      /* printf("i: %d\n", i); */
      /* printf("j: %d\n", j); */
      assert_int64_eq(abs(i % j), mod(i, j));
    }
  }
  return 0;
}

static char *test_divide() {
  // preliminary hand-made tests
  assert_int64_eq(3,  divide(10, 3));
  assert_int64_eq(2,  divide(10, 4));
  assert_int64_eq(-2,  divide(-11, 5));
  // automated tests - does it behave like C's div?
  for (int64_t i = -1000; i < 1000; i++) {
    for (int64_t j = 1; j < 10; j++) {
      /* printf("i: %d\n", i); */
      /* printf("j: %d\n", j); */
      assert_int64_eq(i / j, divide(i, j));
    }
  }
  return 0;
}

static char *all_tests() {
  run_test(test_mod);
  run_test(test_divide);
  return 0;
}

int main(void) { test_main(all_tests); }
