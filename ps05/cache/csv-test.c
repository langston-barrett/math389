#include <stdint.h>
#include <stdio.h>
#include "../minunit.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_csv() {
  return 0;
}

static char *all_tests() {
  run_test(test_csv);
  return 0;
}

int main(void) { test_main(all_tests); }
