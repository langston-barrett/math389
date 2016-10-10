#include <stdint.h>
#include <stdio.h>
#include "../../minunit.h"

#include "./csv.gen.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_csv() {
  double arr1[] = {1, 2, 3};
  int64_t arr2[] = {9, 8, 7};
  assert_str_eq("1.000000,9\n2.000000,8\n3.000000,7\n", csv(arr1, arr2, 3));
  return 0;
}

static char *test_str_to_file() {
  assert(str_to_file("./tmp", "gatlin"));
  assert_int64_eq(0, remove("./tmp"));
  return 0;
}

static char *all_tests() {
  run_test(test_csv);
  run_test(test_str_to_file);
  return 0;
}

int main(void) { test_main(all_tests); }
