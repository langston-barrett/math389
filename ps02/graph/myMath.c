#include "myMath.h"

double powerOf(double x, unsigned int n) {
  double p = 1.0;
  while (n > 0) {
    if ((n & 0x1) == 0x1) {
      p *= x;
    }
    x = x * x;
    n >>=  1;
  }
  return p;
}


