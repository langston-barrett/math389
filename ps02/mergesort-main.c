#include <stdio.h>
#include <stdlib.h>
#include "mergesort.gen.h"

int main(int argc, char **argv) {
  int n;
  int* data;

  if (argc <= 1) {
    fprintf(stderr,"usage: %s <size of data>\n", argv[0]);
    return -1;
  }
  n = atoi(argv[1]);

  data = fake_data(n);

  printf("unsorted: ");
  print_array(data, n);

  merge_sort(data,n);

  printf("sorted:   ");
  print_array(data, n);

  return 0;
}
