#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// conversions ('0' -> 0, 7 -> '7')
int char_to_int(char c) { return c - '0'; }
char int_to_char(int i) { return i + '0'; }

// add/subtract the decimal values of characters together, producing an int
int add_chars(char c1, char c2) { return char_to_int(c1) + char_to_int(c2); }
int sub_chars(char c1, char c2) { return char_to_int(c1) - char_to_int(c2); }

// return the longer/shorter string.
// on a tie in lengths, max will return the bigger ASCII value.
char *strmax(char *s1, char *s2) { if (strcmp(s1, s2) > 0) return s1; return s2; }
char *strmin(char *s1, char *s2) { if (strcmp(s1, s2) < 0) return s1; return s2; }

char *remove_leading_zero(char* s) {
  if (s[0] != '0') {
    return s;
  }
  char *buf = malloc(sizeof(s) - sizeof(char));
  strcpy(buf, s + sizeof(char));
  return buf;
}

char *sum(char *src1, char *src2) {
  int carry = 0;
  int i; // loop index

  // our bigger buffer needs one more digit for a possible carry
  char *bigger = malloc(sizeof(strmax(src1, src2)) + sizeof(char));
  char *smaller = malloc(sizeof(strmin(src1, src2)));
  strcpy(bigger + sizeof(char), strmax(src1, src2));
  strcpy(smaller, strmin(src1, src2));
  bigger[0] = '0';
  int big_len = strlen(bigger);
  int sml_len = strlen(smaller);

  for (i = 0; i < sml_len; i++) {
    // the strings need to be independently indexed from their last digit
    // because they are different lengths
    int i_big = big_len - i - 1;
    int i_sml = sml_len - i - 1;

    int result = add_chars(bigger[i_big], smaller[i_sml]);
    int digit = (result + carry) % 10;
    carry = (result + carry) / 10;
    bigger[i_big] = int_to_char(digit);
  }

  if (carry > 10) { printf("You've made a grave error: %d", carry); }

  // keep carrying
  for (i = sml_len; carry > 0 && i < big_len; i++) {
    int result = add_chars(bigger[big_len - i - 1], int_to_char(carry));
    int digit = result % 10;
    carry = result / 10;
    bigger[big_len - i - 1] = int_to_char(digit);
  }

  return remove_leading_zero(bigger);
}

char *difference(char *src1, char *src2) {
  char *i = "not yet implemented";
  return i;
}
