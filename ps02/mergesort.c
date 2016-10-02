#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int i, int j) { if (i < j) return j; return i; }
int min(int i, int j) { if (i > j) return j; return i; }

// print an array in a recognizable text format, end with \n
void print_array(int *arr, int n) {
  printf("[%d", arr[0]);
  for (int i = 1; i < n - 1; i++) {
    printf(", %d", arr[i]);
  }
  printf(", %d]\n", arr[n-1]);
}

// actually merge two subarrays based on bin size and position
void mer(int *arr, int* dest, int bin_size, int portion) {
  // one index for the beginning of each portion of the array
  int i1 = portion * (2 * bin_size);
  int i2 = i1 + bin_size;
  // these are the limits
  int i1_max = i2;
  int i2_max = i2 + bin_size;
  // where to start putting values in the destination
  int dest_offset = i1_max - bin_size;
  int i3 = 0; // how many we've put in the destination

  while (i1 < i1_max || i2 < i2_max) {
    int dest_index = dest_offset + i3;
    // if we're all the way through the i1 section or
    // if the i2 section has the lower value, use the i2 index
    if (!(i1 < i1_max) || (i2 < i2_max && arr[i2] < arr[i1])) {
      dest[dest_index] = arr[i2];
      i2++;
    } else {
      dest[dest_index] = arr[i1];
      i1++;
    }
    i3++;
  }
}

// loop and increase current position in array (portion)
void merge(int *arr, int n, int *dest, int bin_size) {
  int positions = n / (bin_size * 2); // each run of mer sorts two bins together
  int position;
  for (position = 0; position < positions; position++) {
    mer(arr, dest, bin_size, position);
  }
}

// loop and increase bin size
void merge_sort(int *arr, int n) {
  int *dest = malloc(sizeof(arr));
  for (int i = 0; i < n; i++) {
    dest[i] = -999; // readability
  }

  for (int bin_size = 1; bin_size <= n; bin_size = bin_size * 2) {
    merge(arr, n, dest, bin_size);
    // print_array(dest, n);
  }
  for (int i = 0; i < n; i++) {
    arr[i] = dest[i];
  }
}

// Build the reverse permutation of 1 through n
int *fake_data(int n) {
  int* data = (int *)malloc(n*sizeof(int));
  for (int i=0; i<n; i++) {
    data[i] = n-i;
  }

  return data;
}
