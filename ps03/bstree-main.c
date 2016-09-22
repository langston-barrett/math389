#include <stdio.h>
#include "bstree.gen.h"

int main(void) {
  bstree *bst = rand_bst();
  printf("Forward: ");
  forward_output(bst);
  printf("Backward: ");
  reverse_output(bst);
}
