#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../minunit.h"
#include "./sand-lib.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_new_pile() {
  for (uint16_t i = 1; i < 100; i++)
    for (uint16_t j = 1; j < 100; j++)
      assert(new_pile(i, j, i, 4) != NULL);
  return 0;
}

static char *test_step_square() {
  // the center will always be settled after the first step iff
  // the center height < the max height
  for (uint16_t i = 1; i < 100; i++)
    for (uint16_t j = 1; j < 100; j++)
      assert(step_square(new_pile(i, j, i, 4*i), new_pile(i, j, 1, 4*i), i / 2, j / 2));
  return 0;
}

// this provides more testing for step_square
static char *test_pile_to_string() {
  pile_ *singleton = new_pile(1, 1, 1, 4);
  assert_str_eq("1\n", pile_to_string(singleton, false));

  pile_ *nine = new_pile(3, 3, 9, 4);
  assert_str_eq("000\n090\n000\n", pile_to_string(nine, false));

  // this will only remove the center, not step the others
  pile_ *step_src = new_pile(5, 5, 4, 4);
  pile_ *step_dst = new_pile(5, 5, 1, 4);
  step_square(step_src, step_dst, 2, 2);
  assert_str_eq("00000\n00000\n00000\n00000\n00000\n",
                pile_to_string(step_dst, false));
  return 0;
}

static char *all_tests() {
  run_test(test_new_pile);
  run_test(test_step_square);
  run_test(test_pile_to_string);
  return 0;
}

int main(void) { test_main(all_tests); }
