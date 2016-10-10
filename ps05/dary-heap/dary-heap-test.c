#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../minunit.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static bool has_heap_property(dary_heap *heap) {
  assert(heap != NULL);
  assert(heap->array != NULL);

  // TODO: tighter bound
  // loop through all possible parents
  for (int64_t parent = 0; parent < heap->max_size; parent++) {
    parent_value = heap->array[parent];
    // loop through each of their children
    for (int64_t which = 0; which < d; which++) {
      int64_t child_index = child_index(heap, parent, which);
      // as long as this child has a valid index
      if (child_index < heap->max_size) {
        int64_t child_value = heap->array[child_index];
        assert_int64_lt(child_value, parent_value);
      }
      // otherwise, this node won't have any valid children
      else {
        break;
      }
    }
  }
}

static char *test_heap_property() {
  return 0;
}

static char *all_tests() {
  run_test(test_heap_property);
  return 0;
}

int main(void) { test_main(all_tests); }
