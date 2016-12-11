/*
 * Harness for testing with KLEE.
 */
#include <klee/klee.h>
#include <assert.h>
#include <stdint.h>
#include "./sand.h"
#include "./sand-lib.gen.h"


void test_new_pile() {
  uint64_t i;
  uint64_t j;
  uint64_t k;
  uint64_t l;

  klee_make_symbolic(&i, sizeof(i), "i");
  klee_make_symbolic(&j, sizeof(j), "j");
  klee_make_symbolic(&k, sizeof(k), "k");
  klee_make_symbolic(&l, sizeof(l), "l");

  klee_assume(i > 0);
  klee_assume(j > 0);
  klee_assume(k > 0);
  klee_assume(l % 4 == 0);

  klee_assert(new_pile(i, j, k, l) != NULL);
}

void test_evolve() {
  uint64_t rows;
  uint64_t cols;
  uint64_t center;
  uint64_t max;

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

  pile *pile = new_pile(rows, cols, center, max);
  klee_assert(evolve(pile, false, false) != NULL);
}

int main(int argc, char **argv) {
  test_new_pile();
  test_evolve();
  return 0;
}
