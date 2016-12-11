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
#include "./sand-lib.gen.h"

#define SIZE_LOWER_BOUND 50
#define SIZE_UPPER_BOUND 55
#define CENTER_HEIGHT 20000

const uint8_t MAX_HEIGHT = 4;

char *mycsv(bool *threaded, uint64_t *size, clock_t *time, int64_t length) {
  uint64_t line_size = 100; // guess
  char *to_return = malloc(line_size * length * sizeof(char));
  char line[line_size];
  strcat(to_return, "threaded,size,time\n");
  for (int64_t i = 0; i < length; i++) {
    // true or false?
    char *tf = malloc(7*sizeof(char));
    tf[0] = '\000';
    if (threaded[i])
      sprintf(tf, "%s", "true");
    else
      sprintf(tf, "%s", "false");

    sprintf(line, "%s,%lu,%li\n", tf, size[i], time[i]);
    strcat(to_return, line);
  }
  return to_return;
}


int main() {
  pile *pile;
  uint64_t index = 0;

  printf("Testing different grid sizes...\n");

  // how many times will we loop/measure?
  uint64_t loops = (SIZE_UPPER_BOUND - SIZE_LOWER_BOUND) *
                   (SIZE_UPPER_BOUND - SIZE_LOWER_BOUND);
  uint64_t *sizes = malloc(loops * sizeof(double));
  assert(sizes != NULL);
  clock_t *times = malloc(loops * sizeof(clock_t));
  assert(times != NULL);
  bool *threaded = malloc(loops * sizeof(double));
  assert(threaded != NULL);

  for (uint64_t i = SIZE_LOWER_BOUND; i < SIZE_UPPER_BOUND; i++) {
    for (uint64_t j = SIZE_LOWER_BOUND; j < SIZE_UPPER_BOUND; j++) {
      printf("i: %lu, j: %lu\n", i, j);

      // times w/o threads
      pile = new_pile(i, j, CENTER_HEIGHT, MAX_HEIGHT);
      clock_t begin = clock();
      evolve(pile, false, false);
      clock_t end = clock();

      /* printf("here\n"); */

      // record times
      times[index] = end - begin;
      sizes[index] = i * j;
      threaded[index] = false;
      index++;

      /* printf("here2\n"); */

      // times w/ threads
      pile = new_pile(i, j, CENTER_HEIGHT, MAX_HEIGHT);
      begin = clock();
      tevolve(pile);
      end = clock();

      /* printf("here3\n"); */

      // record times
      times[index] = end - begin;
      sizes[index] = i * j;
      threaded[index] = true;
      index++;
    }
  }

  char *csv_rep = mycsv(threaded, sizes, times, loops);
  printf("Writing to data.csv\n");
  str_to_file("./data.csv", csv_rep);

  // printf("Testing different center heights...\n");

  return 0;
}
