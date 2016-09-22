#include <stdio.h>
#include <stdlib.h>
#include "../minunit.h"
#include "bstree.gen.h"

int tests_run = 0;

static char *test_min_helper() {
  int keys[] = {81, 231, -10234, 793142, -231, 17234, 37, 218};
  bstree *bst = arr_bst(keys, 7);
  assert("min(tree) != -10234", min_helper(bst->root)->key == -10234);
  free(bst);
  return 0;
}

static char *test_max_helper() {
  int keys[] = {81, 231, -10234, 793142, -231, 17234, 37, 218};
  bstree *bst = arr_bst(keys, 7);
  assert("max(tree) != 793142", max_helper(bst->root)->key == 793142);
  free(bst);
  return 0;
}

static char *test_prev() {
  bstree *bst = rand_bst();

  // ensure the limits are right
  bstnode *min = min_helper(bst->root);
  assert("prev(min(tree)) != null", min->prev == NULL);
  bstnode *max = max_helper(bst->root);
  assert("prev(max(tree)) == null", max->prev != NULL);

  // ensure they're ordered properly
  bstnode *current = max;
  while (current->prev != NULL && current->prev->prev != NULL) {
    assert("prev weren't ordered\n", current->prev->key >= current->prev->prev->key);
    current = current->prev;
  }

  free(bst);
  return 0;
}

static char *test_next() {
  bstree *bst = rand_bst();

  // ensure the limits are right
  bstnode *min = min_helper(bst->root);
  assert("next(min(tree)) == null", min->next != NULL);
  bstnode *max = max_helper(bst->root);
  assert("next(max(tree)) != null", max->next == NULL);

  // ensure they're ordered properly
  bstnode *current = min;
  while (current->next->next != NULL) {
    assert("next weren't ordered", current->next->key <= current->next->next->key);
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
