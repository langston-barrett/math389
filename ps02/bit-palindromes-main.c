#include <stdio.h>
#include <stdlib.h>
#include "./bit-palindromes.gen.h"

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
