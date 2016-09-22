#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include "bstree.h"

bstnode *new_bstnode(int data) {
  bstnode *n = (bstnode *)malloc(sizeof(bstnode));
  n->key = data;
  n->left = NULL;
  n->right = NULL;
  n->next = NULL;
  n->prev = NULL;
  return n;
}

bstree *new_bstree() {
  bstree *self = (bstree *)malloc(sizeof(bstree));
  self->root = NULL;
  return self;
}

void bstree_insert(bstree *self, int value) {
  bstnode *n = self->root;
  bstnode *parent = NULL;
  bool min = true;
  bool max = true;
  bstnode *x = new_bstnode(value);

  // Find the parent node of the value.
  while (n != NULL) {
    parent = n;
    if (value == n->key) {
      return;
    } else if (value < n->key) {
      max = false;
      x->prev = parent;
      n = n->left;
    } else {
      min = false;
      x->next = parent;
      n = n->right;
    }
  }

  // place the new node.
  if (parent == NULL) {
    self->root = x;
  } else if (value < parent->key) {
    parent->left = x;
    x->next = parent;
    if (min) x->prev = NULL;
  } else {
    parent->right = x;
    x->prev = parent;
    if (max) x->next = NULL;
  }

}

// return a random bst
bstree *rand_bst() {
  srand(time(NULL));
  bstree *bst = new_bstree();
  int max = rand();
  for (int i = 0; i < (max % 10000); i++) {
    bstree_insert(bst, rand() % 100000);
  }
  return bst;
}

// make bstree from int[]
bstree *arr_bst(int *keys, int n) {
  bstree *bst = new_bstree();
  for (int i = 0; i < n; i++) {
    bstree_insert(bst, keys[i]);
  }
  return bst;
}

bstnode *max_helper(bstnode *n) {
  assert(n != NULL);
  while (n->right != NULL) {
    n = n->right;
  }
  return n;
}

bstnode *min_helper(bstnode *n) {
  assert(n != NULL);
  while (n->left != NULL) {
    n = n->left;
  }
  return n;
}

bool bstree_is_empty(bstree *self) {
  return (self->root == NULL);
}

int bstree_size_helper(bstnode *n) {
  if (n == NULL) {
    return 0;
  } else {
    return 1 + bstree_size_helper(n->left) + bstree_size_helper(n->right);
  }
}

int bstree_size(bstree *self) {
  return bstree_size_helper(self->root);
}

bool bstree_contains(bstree *self, int value) {
  bstnode *n = self->root;
  while (n != NULL) {
    if (value == n->key) {
      return true;
    } else if (value < n->key) {
      n = n->left;
    } else {
      n = n->right;
    }
  }
  return false;
}

void forward_output(bstree *self) {
  bstnode *node = min_helper(self->root);
  printf("[%d", node->key);
  while (node->right != NULL) {
    node = node->right;
    printf(", %d", node->key);
  }
  printf("]\n");
}

void reverse_output(bstree *self) {
  bstnode *node = min_helper(self->root);
  printf("[%d", node->key);
  while (node->right != NULL) {
    node = node->right;
    printf(", %d", node->key);
  }
  printf("]\n");
}

void bstree_remove_helper(bstnode **px, int value) {
  if (*px == NULL) {
    return;
  } else {
    if (value == (*px)->key) {

      // Value is at this node. Remove it.
      if ((*px)->right == NULL) {

        // Remove this node.
        (*px) = (*px)->left;
      } else {

        // Replace key of this node.
        int k = min_helper((*px)->right)->key;
        (*px)->key = k;
        // Remove the replacement key's node.
        bstree_remove_helper(&((*px)->right), k);
      }
    } else if (value < (*px)->key) {

      // Delete in left subtree.
      bstree_remove_helper(&((*px)->left), value);
    } else {

      // Delete in right subtree.
      bstree_remove_helper(&((*px)->right), value);
    }
  }
}

void bstree_remove(bstree *self, int value) {
  bstree_remove_helper(&self->root, value);
}

void bstree_output_helper(bstnode *x) {
  if (x == NULL) {
    printf(".");
  } else {
    printf("[%d ", x->key);
    bstree_output_helper(x->left);
    printf(" ");
    bstree_output_helper(x->right);
    printf("]");
  }
}

void bstree_output(bstree *self) {
  bstree_output_helper(self->root);
  printf("\n");
}
