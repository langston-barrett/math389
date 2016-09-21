#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "decimal-sum.h"
#include "../minunit.h"

int tests_run = 0;

static bool streq(char *s1, char *s2) { return (strcmp(s1, s2) == 0); }

static char *test_char_to_int() {
  assert("char_to_int(0) != 0", char_to_int('0') == 0);
  assert("char_to_int(1) != 1", char_to_int('1') == 1);
  assert("char_to_int(2) != 2", char_to_int('2') == 2);
  assert("char_to_int(3) != 3", char_to_int('3') == 3);
  assert("char_to_int(4) != 4", char_to_int('4') == 4);
  assert("char_to_int(5) != 5", char_to_int('5') == 5);
  assert("char_to_int(6) != 6", char_to_int('6') == 6);
  assert("char_to_int(7) != 7", char_to_int('7') == 7);
  assert("char_to_int(9) != 9", char_to_int('9') == 9);
  return 0;
}

static char *test_int_to_char() {
  assert("int_to_char(0) != 0", int_to_char(0) == '0');
  assert("int_to_char(1) != 1", int_to_char(1) == '1');
  assert("int_to_char(2) != 2", int_to_char(2) == '2');
  assert("int_to_char(3) != 3", int_to_char(3) == '3');
  assert("int_to_char(4) != 4", int_to_char(4) == '4');
  assert("int_to_char(5) != 5", int_to_char(5) == '5');
  assert("int_to_char(6) != 6", int_to_char(6) == '6');
  assert("int_to_char(7) != 7", int_to_char(7) == '7');
  assert("int_to_char(8) != 8", int_to_char(8) == '8');
  assert("int_to_char(9) != 9", int_to_char(9) == '9');
  return 0;
}

static char *test_add_chars() {
  assert("add_chars(0, 0) != 0", add_chars('0', '0') == 0);
  assert("add_chars(1, 0) != 1", add_chars('1', '0') == 1);
  assert("add_chars(0, 1) != 1", add_chars('0', '1') == 1);
  assert("add_chars(1, 1) != 2", add_chars('1', '1') == 2);
  assert("add_chars(9, 1) != 10", add_chars('9', '1') == 10);
  return 0;
}

/* static char *test_max() { */
/*   assert("max(0, 0) != 0", max(0, 0) == 0); */
/*   assert("max(1, 0) != 1", max(1, 0) == 1); */
/*   assert("max(0, 1) != 1", max(0, 1) == 1); */
/*   assert("max(1, 1) != 1", max(1, 1) == 1); */
/*   assert("max(9, 1) != 9", max(9, 1) == 9); */
/*   return 0; */
/* } */

/* static char *test_min() { */
/*   assert("min(0, 0) != 0", min(0, 0) == 0); */
/*   assert("min(1, 0) != 0", min(1, 0) == 0); */
/*   assert("min(0, 1) != 0", min(0, 1) == 0); */
/*   assert("min(1, 1) != 1", min(1, 1) == 1); */
/*   assert("min(9, 1) != 1", min(9, 1) == 1); */
/*   return 0; */
/* } */

static char *test_remove_leading_zero() {
  assert("remove_leading_zero(0) != ''", streq(remove_leading_zero("0"), ""));
  assert("remove_leading_zero(05) != '5'", streq(remove_leading_zero("05"), "5"));
  assert("remove_leading_zero(50) != '50'", streq(remove_leading_zero("50"), "50"));
  return 0;
}

static char *test_sum() {
  assert("sum(0, 0) != 0", streq(sum("0", "0"), "0"));
  assert("sum(1, 0) != 1", streq(sum("1", "0"), "1"));
  assert("sum(0, 1) != 1", streq(sum("0", "1"), "1"));
  assert("sum(1, 1) != 2", streq(sum("1", "1"), "2"));
  assert("sum(9, 1) != 10", streq(sum("9", "1"), "10"));
  assert("sum(19, 1) != 20", streq(sum("19", "1"), "20"));
  assert("sum(9999999, 9999999) != 19999998", streq(sum("9999999", "9999999"), "19999998"));
  /* assert("sum(1234567890, 9999999) != 2345679001", streq(sum("1234567890", "1111111111"), "2345679001")); */
  /* assert("sum(4294967296, 4294967296) != 8589934592", streq(sum("4294967296", "4294967296"), "8589934592")); */
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
