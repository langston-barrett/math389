#define BIG_ENOUGH 100

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// returns an exit code
extern uint64_t dc(int64_t *calc_stack);

int main() {
  int64_t *ptr = malloc(BIG_ENOUGH * sizeof(int64_t));
  if (ptr == NULL) {
    printf("Couldn't allocate memory");
    return 1;
  }
  int64_t exit_code = dc(ptr);
  // interpret the exit code
  switch (exit_code) {
  case 0:
    printf("Exited normally.\n");
    break;
  default:
    printf("Invalid character '%c'. Accepted: 0123456789pdx+-*/%%^.\n",
           (int)exit_code);
    break;
  }
  return exit_code;
}
