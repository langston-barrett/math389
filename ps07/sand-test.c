#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../minunit.h"
#include "./sand.gen.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_new_pile() {
  for (uint64_t i = 1; i < 100; i++)
    for (uint64_t j = 1; j < 100; j++)
      assert(new_pile(i, j, i) != NULL);
  return 0;
}

static char *test_step_square() {
  // the center will always be settled after the first step
  for (uint64_t i = 1; i < 100; i++)
    for (uint64_t j = 1; j < 100; j++)
      assert(step_square(new_pile(i, j, i), new_pile(i, j, 1), i / 2, j / 2,
                         4 * i));
  return 0;
}

// this provides more testing for step_square
static char *test_pile_to_string() {
  pile *singleton = new_pile(1, 1, 1);
  assert_str_eq("1\n", pile_to_string(singleton, false));

  pile *nine = new_pile(3, 3, 9);
  assert_str_eq("000\n090\n000\n", pile_to_string(nine, false));

  // this will only remove the center, not step the others
  pile *step_src = new_pile(5, 5, 4);
  pile *step_dst = new_pile(5, 5, 1);
  step_square(step_src, step_dst, 2, 2, 4);
  assert_str_eq("00000\n00000\n00000\n00000\n00000\n",
                pile_to_string(step_dst, false));
  return 0;
}

static char *test_step() {
  // a pile with a center < max_height is always settled
  for (uint64_t i = 1; i < 100; i++)
    for (uint64_t j = 1; j < 100; j++)
      assert(step(new_pile(i, j, i), new_pile(i, j, 1), 4 * i));

  // a pile with a center > max_height is never settled
  for (uint64_t i = 1; i < 100; i++)
    for (uint64_t j = 1; j < 100; j++)
      assert(!step(new_pile(i, j, 8 * i), new_pile(i, j, 1), 4 * i));

  pile *step_src = new_pile(5, 5, 4);
  pile *step_dst = new_pile(5, 5, 1);
  step(step_src, step_dst, 4);
  assert_str_eq("00000\n00100\n01010\n00100\n00000\n",
                pile_to_string(step_dst, false));
  return 0;
}

static char *test_evolve() { return 0; }

static char *all_tests() {
  run_test(test_new_pile);
  run_test(test_pile_to_string);
  run_test(test_step_square);
  run_test(test_step);
  run_test(test_evolve);
  return 0;
}

int main(void) { test_main(all_tests); }
