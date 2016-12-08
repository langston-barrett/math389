#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./sand.gen.h"

#define SLEEP 10000

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

/*@ requires c >= 48 && c <= 57;
  @ assigns \nothing;
  @ ensures \result < 10;
*/
uint8_t char_to_int(char c) { return c - '0'; }
/*@ requires i < 10;
  @ assigns \nothing;
  @ ensures \result >= 48 && \result <= 57;
*/
char int_to_char(uint8_t i) { return i + '0'; }

// can't ensure postconditions on code that calls stdlib functions
/*@ requires \valid(color) && \valid(text); */
void color_printf(char *color, char *text) { printf("%s%s", color, text); }

// can't ensure post-conditions on code that calls stdlib functions
/*@ requires \valid(color) && \valid(str) && valid_string_src: valid_read_string(str);
  @ ensures \valid(color);
  @ ensures \valid(\result);
 */
char *maybe_colorize(char *color, char *str, bool really) {
  if (really) {
    char *new_str = malloc((10+strlen(str))*sizeof(char));
    sprintf(new_str, "%s%s", color, str);
    return new_str;
  }
  return str;
}

/*@ requires \valid(pile) && \valid(pile->grid);
  @ assigns \nothing;
  @ ensures \valid(\result);
*/
char *pile_to_string(pile *pile, bool color) {
  char *str = malloc((10*pile->rows*pile->cols)*sizeof(char));
  assert(str != NULL); // make sure we've got memory
  str[0] = '\000'; // zero it

  /*@ loop invariant 0 <= i <= pile->rows;
    @ loop invariant \valid(pile) && \valid(pile->grid);
  */
  for (uint64_t i = 0; i < pile->rows; i++) {
    /*@ loop invariant 0 <= j <= pile->cols;
      @ loop invariant \valid(pile) && \valid(pile->grid);
    */
    for (uint64_t j = 0; j < pile->cols; j++) {
      switch (pile->grid[i][j]) {
      case 0:
        strcat(str, maybe_colorize(RESET, "0", color));
        break;
      case 1:
        strcat(str, maybe_colorize(BLUE, "1", color));
        break;
      case 2:
        strcat(str, maybe_colorize(GREEN, "2", color));
        break;
      case 3:
        strcat(str, maybe_colorize(YELLOW, "3", color));
        break;
      }
      if (pile->grid[i][j] > 3) {
        if (pile->grid[i][j] < 10) {
          char *char_str = malloc(2*sizeof(char));
          char_str[0] = int_to_char(pile->grid[i][j]);
          char_str[1] = '\000';
          strcat(str, maybe_colorize(RED, char_str, color));
          free(char_str);
        }
        else strcat(str, maybe_colorize(RED, "?", color));
      }
    }
    strcat(str, "\n");
  }

  return str;
}

/*@ requires \valid(pile) && \valid(pile->grid);
  @ ensures \valid(pile) && \valid(pile->grid);
 */
void print_pile(pile *pile, bool color) {
  printf("\033[2J\033[1;1H"); // clear the screen
  printf("%s", pile_to_string(pile, color));
}

pile *new_pile(uint64_t rows, uint64_t cols, uint64_t center) {
  /* assert(rows != 0); */
  /* assert(cols != 0); */
  /* assert(center != 0); */

  pile *pile = malloc(sizeof(pile));
  /* assert(pile != NULL); */

  pile->rows = rows;
  pile->cols = cols;

  // allocate the grid
  pile->grid = malloc(rows * sizeof(uint64_t));
  for (uint64_t i = 0; i < rows; i++) {
    pile->grid[i] = malloc(cols * sizeof(uint64_t));
    /* assert(pile->grid[i] != NULL); // make sure we have the memory */
    for (uint64_t j = 0; j < cols; j++) { // zero it out
      pile->grid[i][j] = 0;
    }
  }

  uint64_t center_row = rows / 2;
  uint64_t center_col = cols / 2;
  pile->grid[center_row][center_col] = center;

  return pile;
}

// this is the part based on Jim's pseudocode
// returns whether or not this square is now settled
// preconditions are verified in step()
/*@ requires \valid(src) && \valid(src->grid) && \valid(dst) && \valid(dst->grid);
  @ assigns dst->grid[row][col];
  @ ensures \valid(src) && \valid(src->grid) && \valid(dst) && \valid(dst->grid);
 */
bool step_square(pile *src, pile *dst, uint64_t row, uint64_t col, uint64_t max) {
  if (src->grid[row][col] >= max)
    dst->grid[row][col] = src->grid[row][col] - max;
  else
    dst->grid[row][col] = src->grid[row][col];

  if (row > 0 && src->grid[row - 1][col] >= max) // north
    dst->grid[row][col] = dst->grid[row][col] + (max / 4);
  if (col > 0 && src->grid[row][col - 1] >= max) // east
    dst->grid[row][col] = dst->grid[row][col] + (max / 4);
  if (row < src->rows - 1 && src->grid[row + 1][col] >= max) // south
    dst->grid[row][col] = dst->grid[row][col] + (max / 4);
  if (col < src->cols - 1 && src->grid[row][col + 1] >= max) // west
    dst->grid[row][col] = dst->grid[row][col] + (max / 4);

  // if we're still over max height in any cell, we're not done
  if (dst->grid[row][col] >= max)
    return false;
  return true;
}

// step a pile forward in time. returns whether or not the pile is settled.
/*@ requires \valid(src) && \valid(src->grid);
  @ requires \valid(dst) && \valid(dst->grid);
  @ requires max_height % 4 == 0;
  @ ensures \valid(src) && \valid(src->grid) && \valid(dst) && \valid(dst->grid);
*/
bool step(pile *src, pile *dst, uint64_t max_height) {
  assert(src != NULL);
  assert(src->grid != NULL);
  assert(dst != NULL);
  assert(dst->grid != NULL);

  bool done = true;
  /*@ loop invariant 0 <= i <= src->rows;
    @ loop invariant \valid(src) && \valid(src->grid);
    @ loop invariant \valid(dst) && \valid(dst->grid);
  */
  for (uint64_t i = 0; i < src->rows; i++) {
    /*@ loop invariant 0 <= j <= src->cols;
      @ loop invariant \valid(src) && \valid(src->grid);
      @ loop invariant \valid(dst) && \valid(dst->grid);
    */
    for (uint64_t j = 0; j < src->cols; j++) {
      if (!step_square(src, dst, i, j, max_height))
        done = false;
    }
  }

  assert(src != NULL);
  assert(src->grid != NULL);
  assert(dst != NULL);
  assert(dst->grid != NULL);
  return done;
}

// take a pile forward until it settles. printing optional. color optional.
/*@ requires \valid(src) && \valid(src->grid);
  @ requires max_height % 4 == 0;
  @ ensures \valid(src) && \valid(src->grid);
*/
pile *evolve(pile *src, uint64_t max_height, bool print, bool color) {
  assert(src != NULL);
  assert(src->grid != NULL);
  assert(max_height % 4 == 0);

  // we'll synchronously switch between data in this pile and the other
  pile *dst = new_pile(src->rows, src->cols, 1);
  assert(dst != NULL); // make sure we have the memory

  /*@ loop invariant \valid(src) && \valid(src->grid);
    @ loop invariant \valid(dst) && \valid(dst->grid);
  */
  while (!step(src, dst, max_height)) { // while we're still going
    if (print) { // optionally print
      print_pile(dst, color);
      usleep(SLEEP);
    }

    pile *tmp = src; // switch src and dst
    src = dst;
    dst = tmp;
  }

  assert(src != NULL);
  assert(src->grid != NULL);
  assert(dst != NULL);
  assert(dst->grid != NULL);
  return dst;
}
