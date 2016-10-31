#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

const int64_t shuffles = 1000;

/**
   @brief Same as random_array, just parameterized on the number of shuffles.
   @param size The size of the array.
   @param shuffles How many times to shuffle the elements.
*/
int64_t *random_array2(int64_t size, int64_t shuffles) {
  srand(time(NULL));
  int64_t *arr = malloc(size*sizeof(int64_t));

  // fill it with its indices
  for (int64_t i = 0; i < size; i++) {
    arr[i] = i;
  }

  // randomly shuffle them
  for (int64_t i = 0; i < shuffles; i++) {
    int64_t index1 = rand() % size;
    int64_t index2 = rand() % size;
    int64_t tmp = arr[index1];
    arr[index1] = arr[index2];
    arr[index2] = tmp;
  }

  return arr;
}

/**
   @brief Build an array of size n filled with its indices, in a random order.
   @param size The size of the array
*/
int64_t *random_array(int64_t size) {
  return random_array2(size, shuffles);
}
