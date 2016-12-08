#ifndef _sand_h
#define _sand_h
#include <stdint.h>
typedef struct pile {
  uint64_t **grid;
  uint64_t rows;
  uint64_t cols;
} pile;
#endif
