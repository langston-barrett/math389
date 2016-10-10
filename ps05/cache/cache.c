#include <stdint.h>

void main() {
}

/* For L1 caches the size is something like 64000 bytes on AMD processors */
void testacache(int64 max_cache) {
  int64 arrayOfTestValues[max_cache];
  for (int step = 0; step < max_cache; step++) {
    arrayOfTestValues[step] = step;
  }
  int64 x = random_int64();
  for (int i = 0; i < max_cache; i++) {
    x = x ^ arrayOfTestValues[i];
  }
  
}
