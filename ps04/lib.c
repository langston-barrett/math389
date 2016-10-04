#include <stdio.h>
#include <stdint.h>

int64_t mul(int64_t n, int64_t m) {
  return n*m;
}

int64_t input_char() {
  char c;
  if (!scanf(" %c",&c)) { // skip initial whitespace
    printf("Error scanning for input.\n");
  }
  return (int64_t) c;
}

void output_int64(int64_t v) {
  printf("%li\n", v);
}
