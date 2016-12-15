#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../ps05/cache/csv.gen.h"
#include "./sand.h"
#include "./tsand.h"
#include "./sand-lib.h"

#define SIZE_LOWER_BOUND 1000
#define SIZE_UPPER_BOUND 10000
#define SIZE_STEP 1000
#define HEIGHT_LOWER_BOUND 1000
#define HEIGHT_UPPER_BOUND 3000
#define HEIGHT_STEP 1000

#define MAX_HEIGHT 4

char *mycsv(bool *threaded, uint64_t *size, uint64_t *height, clock_t *time,
            int64_t length) {
  uint64_t line_size = 100; // guess
  char *to_return = malloc(line_size * length * sizeof(char));
  char line[line_size];
  strcat(to_return, "threaded,size,height,time\n");
  for (int64_t i = 0; i < length; i++) {
    // true or false?
    char *tf = malloc(7 * sizeof(char));
    tf[0] = '\000';
    if (threaded[i])
      sprintf(tf, "%s", "true");
    else
      sprintf(tf, "%s", "false");

    sprintf(line, "%s,%lu,%lu,%li\n", tf, size[i], height[i], time[i]);
    strcat(to_return, line);
  }
  return to_return;
}

int main() {
  pile_ *pile;
  uint64_t index = 0;

  printf("Testing different grid sizes...\n");

  // how many times will we loop/measure?
  uint64_t loops = (SIZE_UPPER_BOUND - SIZE_LOWER_BOUND) *
                   (SIZE_UPPER_BOUND - SIZE_LOWER_BOUND);
  uint64_t *size = malloc(loops * sizeof(uint64_t));
  assert(size != NULL);
  uint64_t *height = malloc(loops * sizeof(uint64_t));
  assert(height != NULL);
  clock_t *time = malloc(loops * sizeof(clock_t));
  assert(time != NULL);
  bool *threaded = malloc(loops * sizeof(bool));
  assert(threaded != NULL);

  for (uint64_t i = SIZE_LOWER_BOUND; i < SIZE_UPPER_BOUND; i = i + SIZE_STEP) {
    printf("i: %lu\n", i);
    for (uint64_t j = SIZE_LOWER_BOUND; j < SIZE_UPPER_BOUND;
         j = j + SIZE_STEP) {
      for (uint64_t k = HEIGHT_LOWER_BOUND; k < HEIGHT_UPPER_BOUND;
           k = k + HEIGHT_STEP) {
        // times w/o threads
        pile = new_pile(i, j, k, MAX_HEIGHT);
        clock_t begin = clock();
        evolve(pile, false, false);
        clock_t end = clock();

        // record times
        time[index] = end - begin;
        height[index] = k;
        size[index] = i * j;
        threaded[index] = false;
        index++;

        // times w/ threads
        pile = new_pile(i, j, k, MAX_HEIGHT);
        begin = clock();
        tevolve(pile);
        end = clock();

        // record times
        time[index] = end - begin;
        height[index] = k;
        size[index] = i * j;
        threaded[index] = true;
        index++;
      }
    }
  }

  char *csv_rep = mycsv(threaded, size, height, time, loops);
  printf("Writing to data.csv\n");
  str_to_file("./data.csv", csv_rep);

  // printf("Testing different center heights...\n");

  return 0;
}
