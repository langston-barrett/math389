#include <stdint.h>
#include <time.h>
#include <stdlib.h>

void main() {}

double *find_cache_size(int stepSize, int64_t expectedCacheSize) {
  int64_t arraySize = expectedCacheSize / 8;
  double *timeArray = malloc(sizeof(double)*arraySize);
  srand(time(NULL));
  for(int i = 0; i < arraySize; i = i + stepSize) {
    int64_t arrayOfTestValues[i];
    /* Seed the array with random values */
    for (int k = 0; k < i; k++) {
      arrayOfTestValues[k] = (int64_t)rand();
    }
    int64_t x = (int64_t)rand();
    /* Warm up the array */
    for (int l = 0; i < 10; l++) {
      for (int k = 0; k < i; k++) {
        x = x ^ arrayOfTestValues[k];
      }
    }
    /* Time the array cache fetching for this i */
    clock_t begin = clock();
    for (int k = 0; k < i; k++) {
      x = x ^ arrayOfTestValues[k];
    }
    clock_t end = clock();
    timeArray[i] = (double)(end - begin) / CLOCKS_PER_SEC;
  }
  return timeArray;
}
