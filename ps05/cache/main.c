#include <stdio.h>
#include <stdlib.h>
#include "./cache-size.gen.h"
#include "./csv.gen.h"
#include "./cache-penalty.gen.h"


const int64_t l1_guess = 512000; // 64kb in bits
const int64_t l2_guess = 4100000; // 512kb in bits
const int64_t l3_guess = 128000000; // 16mb in bits
const int64_t l3_guess2 = 256000000; // 32mb in bits

// from http://www.cpu-world.com/CPUs/Bulldozer/AMD-FX-Series%20FX-8320.html
// Cache: 	L1 data 	L1 instruction 	L2 	L3
// Size: 	8 x 16 KB 	4 x 64 KB 	4 x 2 MB 	8 MB
int main(void) {
  //printf("Attempting to find size of L1 cache...\n");
  //int64_t l1_step_size = 100;
  //int64_t length_times_L1 = l3_guess2 / (8 * l1_step_size);
  //int64_t *sizes = malloc(length_times_L1*sizeof(int64_t));
  //double *arr = find_cache_size(l1_step_size, l3_guess2, 10);

  //sizes[0] = 0;
  //for (int64_t i = 1; i < length_times_L1; i++) {
  //  sizes[i] = sizes[i-1] + l1_step_size;
  //}
  //char *csv_rep = csv(sizes, arr, length_times_L1);
  //printf("Writing to cache-size-data.csv\n");
  //str_to_file("./cache-size-data.csv", csv_rep);
  //return 0;

  // Code for finding the cache penalty
  // My L1 cache size = 256 kb
  // My L2 cache size = 1024 kb
  // My L3 cache size = 6144 kb
  int64_t cachesize = 2050000; // 256 kb in bits
  printf("Attempting to find the penalty for the L1 cache...\n");
  double *arr = find_cache_penalty(cachesize);
  int64_t *sizes = malloc(cachesize*sizeof(int64_t));
  sizes[0] = 0;
  for (int64_t i = 1; i < cachesize; i++) {
    sizes[i] = i;
  }
  char *csv_rep = csv(sizes, arr, cachesize);
  printf("Writing to cache-penalty-data.csv\n");
  str_to_file("./cache-penalty-data.csv", csv_rep);
  return 0;
}
