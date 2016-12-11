#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "./sand-lib.gen.h"

#define SLEEP 100000

bool step(pile *src, pile *dst) {
  return step_region(src, dst, 0, src->rows, 0, src->cols);
}

// take a pile forward until it settles. printing optional. color optional.
pile *evolve(pile *src, bool print, bool color) {
  assert(src != NULL);
  assert(src->grid != NULL);
  assert(src->max % 4 == 0);

  // we'll synchronously switch between data in this pile and the other
  pile *dst = new_pile(src->rows, src->cols, 1, src->max);
  assert(dst != NULL); // make sure we have the memory

  while (!step(src, dst)) { // while we're still going
    if (print) { // optionally print
      print_pile(dst, color);
      usleep(SLEEP);
    }

    pile *tmp = src; // switch src and dst
    src = dst;
    dst = tmp;
  }

  assert(src != NULL);
  assert(src->grid != NULL);
  assert(dst != NULL);
  assert(dst->grid != NULL);
  return dst;
}
