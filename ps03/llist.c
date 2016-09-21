#include "llist.h"

lnode *new_lnode(char *key, int64_t value) {
  lnode *n = (lnode *)malloc(sizeof(lnode));
  n->key = key;
  n->value = value;
  return n;
}

llist *new_llist(void) {
  llist *self = (llist *)malloc(sizeof(llist));
  self->first = NULL;
  return self;
}

bool llist_is_empty(llist *self) {
  return (self->first == NULL);
}

int64_t llist_length(llist *self) {
  int64_t l = 0;
  lnode *n = self->first;
  while (n != NULL) {
    n = n->next;
    l++;
  }
  return l;
}

bool llist_contains_key(llist *self, char *key) {
  lnode *n = self->first;
  while (n != NULL) {
    if (n->key == key) {
      return true;
    }
    n = n->next;
  }
  return false;
}

bool llist_contains_value(llist *self, int64_t value) {
  lnode *n = self->first;
  while (n != NULL) {
    if (n->value == value) {
      return true;
    }
    n = n->next;
  }
  return false;
}

void llist_insert(llist *self, char *key, int64_t value) {
  if (!llist_contains_key(self, key)) {
    lnode *n = new_lnode(key, value);
    n->next = self->first;
    self->first = n;
  }
}

lnode *llist_lookup(llist *self, char *key) {
  lnode *n = self->first;
  while (n != NULL) {
    if (n->key == key) {
      return n;
    }
    n = n->next;
  }
  return NULL;
}

void llist_remove(llist *self, char *key) {
  lnode *n = self->first;
  lnode *p = NULL;
  while (n != NULL) {
    if (n->key == key) {
      if (p == NULL) {
        // Remove from the front.
        self->first = self->first->next;
      } else {
        // Remove from one of the other positions.
        p->next = n->next;
      }
      return;
    }
    p = n;
    n = n->next;
  }
}

int64_t num_chars(int64_t d) {
  int64_t l = 1;
  if (d < 0) {
    l = 2;
    d = -d;
  }
  while (d > 9) {
    l++;
    d = d / 10;
  }
  return l;
}

char *llist_as_string(llist *self) {
  char *s;
  if (llist_is_empty(self)) {
    s = malloc(3);
    sprintf(s, "{}");
    return s;
  } else {
    int64_t commas = llist_length(self)-1;
    int64_t chars = 0;

    // Compute the total length of the value items' characters.
    lnode *n = self->first;
    while (n != NULL) {
      chars += num_chars(n->value);
      n = n->next;
    }
    int64_t l = 2 + (2*commas) + chars;
    s = malloc(l+1);

    // Fill the string.
    sprintf(s, "{");
    chars = 1;

    n = self->first;
    sprintf(s+chars, "%d", n->value);
    chars += num_chars(n->value);
    n = n->next;
    while (n != NULL) {
      sprintf(s+chars, ", %d", n->value);
      chars += 2 + num_chars(n->value);
      n = n->next;
    }
    sprintf(s+chars, "}");

    // Done. Return it.
    return s;
  }
}
