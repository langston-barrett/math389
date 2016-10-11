#include <stdio.h>
#include <stdlib.h>
#include "./cache-size.gen.h"
#include "./csv.gen.h"

// from http://www.cpu-world.com/CPUs/Bulldozer/AMD-FX-Series%20FX-8320.html
// Cache: 	L1 data 	L1 instruction 	L2 	L3
// Size: 	8 x 16 KB 	4 x 64 KB 	4 x 2 MB 	8 MB
int main(void) {
  printf("Attempting to find size of L1 cache...\n");
  int64_t l1_size = 2*512000; // 64kb
  // int64_t l1_size = 512000; // 64kb
  // int64_t l1_size = 320000; // 40kb
  // int64_t l1_size = 256000; // 32kb
  int64_t l1_step_size = 1;
  int64_t length_times_L1 = l1_size / (8 * l1_step_size);
  int64_t *sizes = malloc(length_times_L1*sizeof(int64_t));
  double *arr = find_cache_size(l1_step_size, l1_size, 10);

  sizes[0] = 0;
  for (int64_t i = 1; i < length_times_L1; i++) {
    sizes[i] = sizes[i-1] + l1_step_size;
  }
  char *csv_rep = csv(sizes, arr, length_times_L1);
  printf("Writing to cache-size-data.csv\n");
  str_to_file("./cache-size-data.csv", csv_rep);
  return 0;
}
