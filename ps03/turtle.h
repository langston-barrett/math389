#include <stdint.h>
#include <stdbool.h>

typedef struct _turtle {
  char *name;
  int64_t x;
  int64_t y;
  enum {north, east, south, west} direction;
  int ink;
  bool pen_down;
} turtle;

turtle *new_turtle(char *name);
void forward(turtle *t);
void turn_left(turtle *t);
void turn_right(turtle *t);
void pen_down(turtle *t);
void pen_up(turtle *t);
char *as_string(turtle *t);
void replenish(turtle *t);
