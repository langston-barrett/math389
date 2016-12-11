#ifndef _sand_lib_h
#define _sand_lib_h
#include <stdint.h>
typedef struct pile {
  uint64_t rows;
  uint64_t cols;
  uint64_t max;
  uint64_t **grid;
} pile;
#endif
