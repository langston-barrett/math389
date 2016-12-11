#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../minunit.h"
#include "./sand.h"
#include "./tsand.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_step() {
  // a pile with a center < max is always settled
  for (uint64_t i = 1; i < 100; i++)
    for (uint64_t j = 1; j < 100; j++)
      assert(step(new_pile(i, j, i, 4*i), new_pile(i, j, 1, 4*i)));

  // a pile with a center > max is never settled
  for (uint64_t i = 1; i < 100; i++)
    for (uint64_t j = 1; j < 100; j++)
      assert(!step(new_pile(i, j, 8 * i, 4*i), new_pile(i, j, 1, 4*i)));

  pile *step_src = new_pile(5, 5, 4, 4);
  pile *step_dst = new_pile(5, 5, 1, 4);
  step(step_src, step_dst);
  assert_str_eq("00000\n00100\n01010\n00100\n00000\n",
                pile_to_string(step_dst, false));

  return 0;
}

static char *test_evolve() {
  pile *src = new_pile(5, 5, 500, 4);
  pile *dst = evolve(src, false, false);
  assert_str_eq("02320\n22322\n33033\n22322\n02320\n",
                pile_to_string(dst, false));

  pile *src2 = new_pile(5, 5, 16, 4);
  pile *dst2 = evolve(src2, false, false);
  assert_str_eq("00100\n02120\n11011\n02120\n00100\n",
                pile_to_string(dst2, false));
  return 0;
}

static char *test_tevolve() {
  pile *src = new_pile(5, 5, 4, 4);
  pile *dst = tevolve(src);
  assert_str_eq("00000\n00100\n01010\n00100\n00000\n",
                pile_to_string(dst, false));

  pile *src2 = new_pile(5, 5, 16, 4);
  pile *dst2 = tevolve(src2);
  assert_str_eq("00100\n02120\n11011\n02120\n00100\n",
                pile_to_string(dst2, false));

  pile *src1 = new_pile(5, 5, 500, 4);
  pile *dst1 = tevolve(src1);
  assert_str_eq("02320\n22322\n33033\n22322\n02320\n",
                pile_to_string(dst1, false));

  return 0;
}

static char *all_tests() {
  run_test(test_step);
  run_test(test_evolve);
  run_test(test_tevolve);
  return 0;
}

int main(void) { test_main(all_tests); }
