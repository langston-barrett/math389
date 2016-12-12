#ifndef _sand_lib_h
#define _sand_lib_h
#include <stdint.h>
#pragma clang diagnostic ignored "-Wpadded"
typedef struct _pile {
  uint16_t **grid;
  uint16_t rows; // UINT16_MAX = 2^16-1 ~ 64000
  uint16_t cols;
  uint16_t max;
} pile_;
#endif
