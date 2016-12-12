/*
 * Harness for testing with KLEE.
 */
#include <klee/klee.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "./sand.h"
#include "./sand-lib.gen.h"

void test_evolve() {
  printf("[INFO] testing evolve\n");
  uint16_t rows;
  uint16_t cols;
  uint16_t center;
  uint16_t max;

  // Make the input symbolic.
  klee_make_symbolic(&rows, sizeof(rows), "rows");
  klee_make_symbolic(&cols, sizeof(cols), "cols");
  klee_make_symbolic(&center, sizeof(center), "center");
  klee_make_symbolic(&max, sizeof(max), "max");

  // assume some properties
  klee_assume(rows % 2 == 1);
  klee_assume(cols % 2 == 1);
  klee_assume(center % 4 == 0);
  klee_assume(max % 4 == 0);
  klee_assume(max > 0);

  pile_ *pile = new_pile(rows, cols, center, max);
  klee_assert(evolve(pile, false, false) != NULL);
}

void test_others() {
  /************************** NEW_PILE **************************/
  printf("[INFO] testing new_pile\n");
  uint16_t i;
  uint16_t j;
  uint16_t k;
  uint16_t l;

  klee_make_symbolic(&i, sizeof(i), "i");
  klee_make_symbolic(&j, sizeof(j), "j");
  klee_make_symbolic(&k, sizeof(k), "k");
  klee_make_symbolic(&l, sizeof(l), "l");

  klee_assume(i > 0);
  klee_assume(j > 0);
  klee_assume(k > 0);
  klee_assume(l % 4 == 0);

  pile_ *pile1 = new_pile(i, j, k, l);
  klee_assert(pile1 != NULL);
  free(pile1);

  /************************** STEP_SQUARE **************************/
  printf("[INFO] testing step_square\n");
  // the center will always be settled after the first step if k < 2*l
  klee_assume(k < 2*l);
  pile1 = new_pile(i, j, k, l);
  pile_ *pile2 = new_pile(i, j, k, l);
  klee_assert(step_square(pile1, pile2, i / 2, j / 2));
  free(pile1);
  free(pile2);

  /************************** STEP **************************/
  printf("[INFO] testing step, center < max\n");
  // a pile with a center < max is always settled
  klee_assume(k < l); // this will be "provably false" sometimes
  pile1 = new_pile(i, j, k, l);
  pile2 = new_pile(i, j, k, l);
  klee_assert(step(pile1, pile2));
  free(pile1);
  free(pile2);

  printf("[INFO] testing step, center > max\n");
  // a pile with a center > max is never settled
  uint16_t m;
  klee_make_symbolic(&m, sizeof(m), "m");
  klee_assume(m % 4 == 0);
  klee_assume(k > m);

  pile1 = new_pile(i, j, k, m);
  pile2 = new_pile(i, j, k, m);
  klee_assert(!step(pile1, pile2));
  free(pile1);
  free(pile2);
}

int main(int argc, char **argv) {
  /* test_evolve(); */ // SUPER intensive
  test_others();
  return 0;
}
