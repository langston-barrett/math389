#ifndef _sand_lib_h
#define _sand_lib_h

#include <stdbool.h>
#include <stdint.h>

#pragma clang diagnostic ignored "-Wpadded"

typedef struct _pile {
  uint16_t **grid;
  uint16_t rows; // UINT16_MAX = 2^16-1 ~ 64000
  uint16_t cols;
  uint16_t max;
} pile_;

char *pile_to_string(pile_ *pile, bool color);
void print_pile(pile_ *pile, bool color);
pile_ *new_pile(uint16_t rows, uint16_t cols, uint16_t center, uint16_t max);
bool __attribute__((hot))
step_square(pile_ *src, pile_ *dst, uint16_t row, uint16_t col);
bool __attribute__((nonnull))
step_region(pile_ *src, pile_ *dst, uint16_t row_beg, uint16_t row_end,
            uint16_t col_beg, uint16_t col_end);
#endif
