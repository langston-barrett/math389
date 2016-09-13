#include <stdio.h>

#pragma GCC diagnostic ignored "-Wunused-result"

int main()
{
  int input;
  int i;

  printf("Enter a positive integer: ");
  scanf("%d", &input);

  // print them as we find them instead of accumulating in memory
  printf("The positive factors of %d are: 1", input);
  for (i = 2; i <= input / 2; i++) {
    if (input % i == 0) {
      printf(", %d", i);
    }
  }
}
