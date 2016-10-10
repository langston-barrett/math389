#include <stdint.h>
#include <time.h>
#include <stdlib.h>

void main() {}

/* @brief Create an array of int64s, time the cache accessing those and return
   the timings
   @param cache_size The expected size of the cache
   @return the array representation of the timing of our cache
*/
double *test_a_cache_size(int64_t cache_size) {
  int64_t array_size = (cache_size / 8) + 1;
  int64_t arrayOfTestValues[array_size];
  for (int step = 0; step < array_size; step++) {
    arrayOfTestValues[step] = (int64_t)step;
  }
  /* Warm up the cache by moving through the array once */
  /* We need ot write a function to create a random int64 */
  int64_t x = random_int64();
  for (int i = 0; i < 10 * (array_size); i++) {
    x = x ^ arrayOfTestValues[i];
  }
  /* Time the cache by running through the array ten times */
  x = random_int64();
  double *timerArray[array_size];
  for (int i = 0; i < 10 * (arry_size); i++) {
    clock_t begin = clock();
    x = x ^ arrayOfTestValues[i];
    clock_t end = clock();
    timerArray[i] = (double)(end - begin) / CLOCK_PER_SEC;
  }
  return timerArray;
}

/* @brief Generates a random int and casts as int64
   @return returns the int64_t that was generated
*/
int64_t random_int64() {
  srand(time(NULL));
  /* Should we be using rand()? Do we need a limited range?
     Do our random int64s need to be uniform? */
  int64_t r = (int64_t)rand();
  return r;
}

/* @brief subtracts hitspeed from each time found in our time array
   @param timeArray Our timeArray found in test_a_cache_size
   @param hitspeed How long it takes for the cache to get a hit
   @param array_size The size of our array
   @return the array of our missPenalities
*/
double *findMissPenalities(double *timeArray, double hitSpeed, int64_t array_size) {
  double *missPenalties[array_size];
  for (i = 0; i < array_size, i++) {
    missPenalties[i] = timeArray[i] - hitSpeed;
  }
  return missPenalties;
}
