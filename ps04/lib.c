#include <stdio.h>
#include <stdint.h>

int64_t mul(int64_t n, int64_t m) {
  return n*m;
}

char input_char() {
  char c;
  if (!scanf(" %c",&c)) { // skip initial whitespace
    printf("Error scanning for input.\n");
  }
  return c;
}

void output_int64(int64_t v) {
  printf("%li\n", v);
}

void invalid_char(int64_t code) {
  printf("Invalid character '%c'. Accepted: [0-9]pdx+-*/%%^.\n", (int) code);
}

void quit(int64_t code) {
  switch ((int) code) {
  case 0:
    printf("Exited normally.\n");
    break;
  default:
    printf("Unknown error code: %li\n", code);
  }
}
