#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../minunit.h"
#include "bstree.gen.h"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_min_helper() {
  int keys[] = {81, 231, -10234, 793142, -231, 17234, 37, 218};
  bstree *bst = arr_bst(keys, 7);
  assert(min_helper(bst->root)->key == -10234);
  free(bst);
  return 0;
}

static char *test_max_helper() {
  int keys[] = {81, 231, -10234, 793142, -231, 17234, 37, 218};
  bstree *bst = arr_bst(keys, 7);
  assert(max_helper(bst->root)->key == 793142);
  free(bst);
  return 0;
}

static char *test_prev() {
  bstree *bst = rand_bst();

  // ensure the limits are right
  bstnode *min = min_helper(bst->root);
  assert(min->prev == NULL);
  bstnode *max = max_helper(bst->root);
  assert(max->prev != NULL);

  // ensure they're ordered properly
  bstnode *current = max;
  while (current->prev != NULL && current->prev->prev != NULL) {
    assert(current->prev->key >= current->prev->prev->key);
    current = current->prev;
  }

  free(bst);
  return 0;
}

static char *test_next() {
  bstree *bst = rand_bst();

  // ensure the limits are right
  bstnode *min = min_helper(bst->root);
  assert(min->next != NULL);
  bstnode *max = max_helper(bst->root);
  assert(max->next == NULL);

  // ensure they're ordered properly
  bstnode *current = min;
  while (current->next->next != NULL) {
    assert(current->next->key <= current->next->next->key);
    current = current->next;
  }

  free(bst);
  return 0;
}

static char *all_tests() {
  run_test(test_min_helper);
  run_test(test_max_helper);
  run_test(test_prev);
  run_test(test_next);
  return 0;
}

int main(void) { test_main(all_tests); }
