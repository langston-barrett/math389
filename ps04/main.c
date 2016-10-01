#define BIG_ENOUGH 100

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

extern uint64_t dc(int64_t *calc_stack);

int main() {
  int64_t *ptr = malloc(BIG_ENOUGH * sizeof(int64_t));
  dc(ptr);
  return 0;
}
