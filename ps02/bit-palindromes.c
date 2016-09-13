#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// using the repeated squares method
int ipow(int base, int exp)
{
  int result = 1;
  while (exp) {
    if (exp & 1) {
      result *= base;
    }
    exp >>= 1;
    base *= base;
  }

  return result;
}

bool is_bit_palindrome(int i) {
  while (i > 10) {
    // last digit is easy
    int last_digit = i % 10;

    // first digit is a bit more complex
    int copy = i;
    int counter = 0;
    int first_digit;
    for (counter = 0; copy >= 10; counter++) {
      copy = copy / 10;
    }
    first_digit = copy;

    if (first_digit != last_digit) {
      return false;
    }

    // take the first digit off of i
    i = i - (ipow(10, counter) * first_digit);
    // take the last digit off of i
    i = i / 10;
  }
  return true;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("%s requires one argument.\n", argv[0]);
    return 1;
  }

  int input = (int) strtol(argv[1], NULL, 10);
  if (is_bit_palindrome(input)) {
    printf("%d is palindromic.\n", input);
  } else {
    printf("%d is not palindromic.\n", input);
  }
  return 0;
}
