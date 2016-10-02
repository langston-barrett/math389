#include <stdbool.h>
#include <stdint.h>

// using the repeated squares method
int64_t ipow(int64_t base, int64_t exp)
{
  int64_t result = 1;
  while (exp) {
    if (exp & 1) {
      result *= base;
    }
    exp >>= 1;
    base *= base;
  }

  return result;
}

bool is_bit_palindrome(int64_t i) {
  while (i > 10) {
    // last digit is easy
    int64_t last_digit = i % 10;

    // first digit is a bit more complex
    int64_t copy = i;
    int64_t counter = 0;
    int64_t first_digit;
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
