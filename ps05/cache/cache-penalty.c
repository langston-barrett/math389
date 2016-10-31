#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
   @brief Find the cache penalty
   @param cache_size Cache size in bytes
   @return An array of ints representing the time for individual array accesses
*/
double *find_cache_penalty(int64_t cache_size) {
  // make an array, fill it with values
  int64_t *arr = malloc(2 * cache_size * sizeof(int64_t));
  for (int64_t i = 0; i < cache_size * 2; i++) {
    arr[i] = rand();
  }

  // warm up the cache
  int64_t x = rand();
  for (int64_t i = 0; i < 10; i++) {
    for (int64_t j = 0; j < cache_size; j++) {
      x = x ^ arr[i]; // just a constant time op
    }
  }

  // access the individual values
  double *timerArray = malloc(2 * cache_size * sizeof(double));
  for (int64_t i = 0; i < cache_size * 2; i++) {
    clock_t begin = clock();
    x = x ^ arr[i]; // just a constant time op
    clock_t end = clock();
    timerArray[i] = (double)(end - begin) / CLOCKS_PER_SEC;
  }
  return timerArray;
}
