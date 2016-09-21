#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "turtle.h"

char *direction_to_string(turtle *t) {
  switch (t->direction) {
  case north: return "north"; break;
  case east: return "east"; break;
  case south: return "south"; break;
  default: return "west"; break;
  }
}

turtle *new_turtle(char *name) {
  turtle *self = malloc(sizeof(turtle));
  self->name = malloc(strlen(name)+1);
  strcpy(self->name, name);
  self->x = 0;
  self->y = 0;
  self->direction = east;
  self->ink = 10;
  self->pen_down = false;
  return self;
}

void forward(turtle *t) {
  switch (t->direction) {
  case north: t->y++; break;
  case east: t->x++; break;
  case south: t->y--; break;
  default: t->x--; break;
  }
  if (t->pen_down) t->ink--;
  if (t->ink == 0) pen_up(t);
}

void turn_left(turtle *t) {
  switch (t->direction) {
  case north: t->direction = east; break;
  case east: t->direction = south; break;
  case south: t->direction = west; break;
  default: t->direction = north; break;
  }
}

// three left turns is a right turn ;)
void turn_right(turtle *t) { turn_left(t); turn_left(t); turn_left(t); }

void pen_down(turtle *t) { t->pen_down = true; }
void pen_up(turtle *t) { t->pen_down = false; }

char *as_string(turtle *t) {
  char *str = malloc(50*sizeof(char));
  sprintf(str, "turtle '%s' at (%li, %li) with heading %s", t->name, t->x, t->y, direction_to_string(t));
  return str;
}

void replenish(turtle *t) { t->ink = t->ink + 10; }

int main(void) {
  printf("not yet implemented.\n");
  return 1;
}
