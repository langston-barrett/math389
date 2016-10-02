#include <stdio.h>
#include <stdint.h>
#include "bit-palindromes.gen.h"
#include "../minunit.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_is_bit_palindrome() {
  assert(is_bit_palindrome(100) == false);
  assert(is_bit_palindrome(101) == true);
  assert(is_bit_palindrome(7) == true);
  assert(is_bit_palindrome(777777777) == true);
  assert(is_bit_palindrome(78) == false);
  return 0;
}

static char *all_tests() {
  run_test(test_is_bit_palindrome);
  return 0;
}

int main(void) { test_main(all_tests); }
