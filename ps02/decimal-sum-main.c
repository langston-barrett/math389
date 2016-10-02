#include <stdio.h>
#include "./decimal-sum.gen.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("%s requires two arguments.\n", argv[0]);
    return 1;
  }

  printf("%s + %s = %s\n", argv[1], argv[2], sum(argv[1], argv[2]));
  printf("%s - %s = %s\n", argv[1], argv[2], difference(argv[1], argv[2]));
  return 0;
}
