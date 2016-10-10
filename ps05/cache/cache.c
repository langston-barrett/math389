#include <stdint.h>
#include <time.h>

void main() {
}

/* @brief Create an array of int64s, time the cache accessing those and return the timings
   @param cache_size The expected size of the cache
   @return the array representation of the timing of our cache
*/
double* testacache(int64_t cache_size) {
  int64_t array_size = (cache_size / 8) + 1;
  int64_t arrayOfTestValues[array_size];
  for (int step = 0; step < array_size; step++) {
    arrayOfTestValues[step] = (int64_t)step;
  }
  /* Warm up the cache by moving through the array once */
  /* We need ot write a function to create a random int64 */
  int64_t x = random_int64();
  for (int i = 0; i < 10*(array_size); i++) {
    x = x ^ arrayOfTestValues[i];
  }
  /* Time the cache by running through the array ten times */
  x = random_int64();
  double* timerArray[array_size];
  for (int i =0; i < 10*(arry_size); i++) {
    clock_t begin = clock();
    x = x ^ arrayOfTestValues[i];
    clock_t end = clock();
    timerArray[i] = (double)(end - begin) / CLOCK_PER_SEC;
  }
  return timerArray;
}
