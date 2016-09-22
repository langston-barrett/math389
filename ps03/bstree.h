#ifndef __bstree_h
#define __bstree_h

typedef struct _bstnode {
  int key;
  struct _bstnode *left;
  struct _bstnode *right;
  struct _bstnode *next;
  struct _bstnode *prev;
} bstnode;

typedef struct _bstree {
  bstnode *root;
} bstree;

#endif
