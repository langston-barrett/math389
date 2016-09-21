#ifndef __llist_h
#define __llist_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

typedef struct _lnode {
  char *key;
  int value;
  struct _lnode *next;
} lnode;

typedef struct _llist {
  lnode *first;
} llist;

#endif
