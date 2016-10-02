#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "decimal-sum.h"
#include "../minunit.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static bool streq(char *s1, char *s2) { return (strcmp(s1, s2) == 0); }

static char *test_char_to_int() {
  assert(char_to_int('0') == 0);
  assert(char_to_int('1') == 1);
  assert(char_to_int('2') == 2);
  assert(char_to_int('3') == 3);
  assert(char_to_int('4') == 4);
  assert(char_to_int('5') == 5);
  assert(char_to_int('6') == 6);
  assert(char_to_int('7') == 7);
  assert(char_to_int('9') == 9);
  return 0;
}

static char *test_int_to_char() {
  assert(int_to_char(0) == '0');
  assert(int_to_char(1) == '1');
  assert(int_to_char(2) == '2');
  assert(int_to_char(3) == '3');
  assert(int_to_char(4) == '4');
  assert(int_to_char(5) == '5');
  assert(int_to_char(6) == '6');
  assert(int_to_char(7) == '7');
  assert(int_to_char(8) == '8');
  assert(int_to_char(9) == '9');
  return 0;
}

static char *test_add_chars() {
  assert(add_chars('0', '0') == 0);
  assert(add_chars('1', '0') == 1);
  assert(add_chars('0', '1') == 1);
  assert(add_chars('1', '1') == 2);
  assert(add_chars('9', '1') == 10);
  return 0;
}

/* static char *test_max() { */
/*   assert(max(0, 0) == 0); */
/*   assert(max(1, 0) == 1); */
/*   assert(max(0, 1) == 1); */
/*   assert(max(1, 1) == 1); */
/*   assert(max(9, 1) == 9); */
/*   return 0; */
/* } */

/* static char *test_min() { */
/*   assert(min(0, 0) == 0); */
/*   assert(min(1, 0) == 0); */
/*   assert(min(0, 1) == 0); */
/*   assert(min(1, 1) == 1); */
/*   assert(min(9, 1) == 1); */
/*   return 0; */
/* } */

static char *test_remove_leading_zero() {
  assert(streq(remove_leading_zero("0"), ""));
  assert(streq(remove_leading_zero("05"), "5"));
  assert(streq(remove_leading_zero("50"), "50"));
  return 0;
}

static char *test_sum() {
  assert(streq(sum("0", "0"), "0"));
  assert(streq(sum("1", "0"), "1"));
  assert(streq(sum("0", "1"), "1"));
  assert(streq(sum("1", "1"), "2"));
  assert(streq(sum("9", "1"), "10"));
  assert(streq(sum("19", "1"), "20"));
  assert(streq(sum("9999999", "9999999"), "19999998"));
  /* assert(streq(sum("1234567890", "1111111111"), "2345679001")); */
  /* assert(streq(sum("4294967296", "4294967296"), "8589934592")); */
  return 0;
}

static char *all_tests() {
  run_test(test_char_to_int);
  run_test(test_int_to_char);
  run_test(test_add_chars);
  /* run_test(test_max); */
  /* run_test(test_min); */
  run_test(test_remove_leading_zero);
  run_test(test_sum);
  return 0;
}

int main(void) { test_main(all_tests); }
