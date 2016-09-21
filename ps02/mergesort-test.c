#include <stdio.h>
#include <stdbool.h>
#include "mergesort.h"
#include "../minunit.h"

int tests_run = 0;

static bool sorted(int *arr, int n) {
  for (int i = 0; i < n - 1; i++) {
    if (arr[i] > arr[i+1]) return false;
  }
  return true;
}

static char *test_mer() {
  // fully sort array of 4 with position 0
  int arr[] = {1, 2, 1, 2};
  int dest[4];
  mer(arr, dest, 2, 0);
  assert("array not sorted", sorted(dest, 4));

  // fully sort array of 8 with position 0
  int arr1[] = {1, 2, 3, 3, 1, 5, 10, 20};
  int dest1[8];
  mer(arr1, dest1, 4, 0);
  assert("array not sorted", sorted(dest1, 8));

  // partially sort array of 8 with position 2
  int arr2[] = {5, 2, 3, 1, 1, 3, 1, 4};
  int dest2[] = {9, 9, 8, 9, 0, 0, 0, 0};
  assert("array not sorted", sorted(dest2 + 4, 4)); // only the last 4 are sorted
  assert("array sorted", !sorted(dest2, 4));


  // fully sort array of 16 with position 0 and binsize 8
  int arr3[] = {8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7};
  int dest3[16];
  merge(arr3, 16, dest3, 8);
  assert("array not sorted", sorted(dest3, 16));
  return 0;
}

static char *test_merge() {
  int arr[] = {1, 2, 4, 3, 5, 6, 8, 7};
  int dest[] = {9, 9, 8, 9, 0, 0, 0, 0};
  merge(arr, 8, dest, 1);
  assert("array not sorted", sorted(dest, 8));

  int arr1[] = {5, 6, 7, 8, 1, 2, 3, 4};
  int dest1[] = {9, 9, 8, 9, 0, 0, 0, 0};
  merge(arr1, 8, dest1, 4);
  assert("array not sorted", sorted(dest1, 8));

  int arr2[] = {8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7};
  int dest2[16];
  merge(arr2, 16, dest2, 8);
  assert("array not sorted", sorted(dest2, 16));
  return 0;
}

static char *test_merge_sort() {
  int arr[4] = {3, 2, 1, 0};
  merge_sort(arr, 4);
  print_array(arr, 4);
  assert("array not sorted", sorted(arr, 4));

  int arr1[8] = {1, 5, 1, 52, 123, 321, 0, 99};
  merge_sort(arr1, 8);
  print_array(arr1, 8);
  assert("arr1ay not sorted", sorted(arr1, 8));
  return 0;
}

static char *all_tests() {
  run_test(test_mer);
  run_test(test_merge);
  run_test(test_merge_sort);
  return 0;
}

int main(void) { test_main(all_tests); }
