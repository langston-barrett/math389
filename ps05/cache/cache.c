#include <stdint.h>
#include <time.h>

void main() {
}

/* For L1 caches the size is something like 64000 bytes on AMD processors */
void testacache(int64 max_cache) {
  int64 arrayOfTestValues[max_cache];
  for (int step = 0; step < max_cache; step++) {
    arrayOfTestValues[step] = step;
  }
  /* Warm up the cache by moving through the array once */
  int64 x = random_int64();
  for (int i = 0; i < 10*(max_cache); i++) {
    x = x ^ arrayOfTestValues[i];
  }
  /* Time the cache by running through the array ten times */
  x = random_int64();
  double timerArray[max_cache];
  for (int i =0; i < 10*(max_cache); i++) {
    clock_t begin = clock();
    x = x ^ arrayOfTestValues[i];
    clock_t end = clock();
    timerArray[i] = (double)(end - begin) / CLOCK_PER_SEC;
  }
}
