#include <stdio.h>
#include <stdint.h>
#include "../minunit.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

extern int64_t mod(int64_t dividend, int64_t divisor);

static char *test_mod() {
  assert(mod(10, 3) == 0);
  assert(mod(10, 4) == 2);
  assert(mod(-3, 2) == -1);
  return 0;
}

static char *all_tests() {
  run_test(test_mod);
  return 0;
}

int main(void) { test_main(all_tests); }
