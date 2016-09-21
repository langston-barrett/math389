#include <stdio.h>
#include "bit-palindromes.h"
#include "../minunit.h"

int tests_run = 0;

static char *test_is_bit_palindrome() {
  assert("is_bit_palindrome(100)", is_bit_palindrome(100) == false);
  assert("is_bit_palindrome(101)", is_bit_palindrome(101) == true);
  assert("is_bit_palindrome(7)", is_bit_palindrome(7) == true);
  assert("is_bit_palindrome(77777777)", is_bit_palindrome(777777777) == true);
  assert("is_bit_palindrome(78)", is_bit_palindrome(78) == false);
  return 0;
}

static char *all_tests() {
  run_test(test_is_bit_palindrome);
  return 0;
}

int main(void) { test_main(all_tests); }
