#include <stdint.h>
#include <stdio.h>
#include "../../minunit.h"

#include "./csv.gen.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_csv() {
  int64_t arr1[] = {9, 8, 7};
  double arr2[] = {1, 2, 3};
  assert_str_eq("9,1.000000\n8,2.000000\n7,3.000000\n", csv(arr1, arr2, 3));
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
