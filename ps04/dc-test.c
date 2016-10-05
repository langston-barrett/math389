#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../minunit.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

extern int64_t mul(int64_t dividend, int64_t divisor);
extern int64_t power(int64_t dividend, int64_t divisor);
extern int64_t mod(int64_t dividend, int64_t divisor);
extern int64_t divide(int64_t dividend, int64_t divisor);
extern int64_t decimal_shift(int64_t x, int64_t n);
extern int64_t decimal_construction(int64_t *ptr);

static char *test_mod() {
  // preliminary hand-made tests
  assert_int64_eq(1, mod(10, 3));
  assert_int64_eq(0, mod(10, 5));
  assert_int64_eq(4, mod(10, 6));
  assert_int64_eq(1, mod(9, 2));
  assert_int64_eq(1, mod(-3, 2));
  assert_int64_eq(0, mod(-10, 2));
  assert_int64_eq(1, mod(-10, 3));
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
  assert_int64_eq(3, divide(10, 3));
  assert_int64_eq(2, divide(10, 4));
  assert_int64_eq(-2, divide(-11, 5));
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

// using the repeated squares method
int64_t ipower(int64_t base, int64_t exp) {
  int64_t result = 1;
  while (exp) {
    if (exp & 1) {
      result *= base;
    }
    exp >>= 1;
    base *= base;
  }

  return result;
}

// not shifting should always give the same result
static char *test_decimal_shift_identity() {
  srand(time(NULL));
  for (int64_t i = 0; i < 1000; i++) {
    int64_t random = rand();
    assert_int64_eq(random, decimal_shift(random, 0));
  }
  return 0;
}

// the results from this should match the ones from the assembly
static int64_t c_decimal_shift(int64_t x, int64_t y) { return x * ipower(10, y); }

static char *test_decimal_shift() {
  for (int64_t i = 0; i < 10; i++) {
    /* int64_t max  = 9,223,372,036,854,775,807 has 19 digits */
    for (int64_t j = 0; j < 18; j++) {
      assert_int64_eq(c_decimal_shift(i, j), decimal_shift(i, j));
    }
  }
  return 0;
}

// construct a random array of numbers
static int64_t *rand_array(int64_t length, int64_t mod) {
  int64_t *arr = malloc(length*sizeof(int64_t));
  for (int64_t i = 0; i < 1000; i++) {
    for (int64_t j = 0; j < length; j++) {
      arr[j] = rand() % mod; // just a digit
    }
  }
  return arr;
}

static char *test_decimal_construction() {
  assert_int64_eq(33, decimal_construction((int64_t[]){2, 3, 3}));
  assert_int64_eq(500, decimal_construction((int64_t[]){3, 5, 0, 0}));

  // test random arrays for segfaults, erroneous negative numbers
  for (int64_t i = 0; i < 1000; i++) {
    int64_t *arr = rand_array(rand() % 18 + 1, 10); // < 18 random digits
    assert_int64_lt(-1, decimal_construction(arr));
  }
  return 0;
}

static char *test_mul() {
  for (int64_t i = 0; i < 1000; i++) {
    for (int64_t j = -1000; j < 1000; j++) {
      assert_int64_eq(i * j, mul(i, j));
    }
  }
  return 0;
}

static char *test_power() {
  for (int64_t i = 0; i < 1000; i++) {
    for (int64_t j = 0; j < 1000; j++) {
      if (i % 2 != 0) {
        assert_int64_eq(ipower(i, j), power(i, j));
      }
    }
  }
  return 0;
}

static char *all_tests() {
  run_test(test_mod);
  run_test(test_divide);
  run_test(test_decimal_shift_identity);
  run_test(test_decimal_shift);
  run_test(test_decimal_construction);
  run_test(test_mul);
  run_test(test_power);
  return 0;
}

int main(void) { test_main(all_tests); }
