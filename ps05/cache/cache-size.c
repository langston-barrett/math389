#include <stdint.h>
#include <time.h>
#include <stdio.h> // to print final x value
#include <stdlib.h>

/**
   @brief Find the cache size
   @param step_size The number of elements to increase the size of the array by
   in each trial.
   @param expected_cache_size The maximum expected size of this level of cache,
   measured in bits.
   @return An array of times it took to read through the cache
*/
double *find_cache_size(int step_size, int64_t expected_cache_size, int64_t traversals) {
  srand(time(NULL)); // seed the random generator.

  // each entry is 64 bits = 8 bytes, step size is in entries
  int64_t arraySize = expected_cache_size / (8 * step_size);
  double *times = malloc(arraySize * sizeof(double));

  int64_t x = (int64_t)rand();
  for (int i = 0; i < arraySize; i = i + step_size) {
    // printf("Examining array of length %i\n", i);
    int64_t test_values[i];

    for (int k = 0; k < i; k++) {
      test_values[k] = (int64_t)rand();
    }

    // printf("Timing %li traversals\n", traversals);
    clock_t begin = clock();
    for (int l = 0; l < traversals; l++) {
      for (int k = 0; k < i; k++) {
        x = x ^ test_values[k];
      }
    }

    clock_t end = clock();
    times[i] = (double)(end - begin) / CLOCKS_PER_SEC;
  }

  printf("To prevent compiler optimization: %li\n", x);

  return times;
}
