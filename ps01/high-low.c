#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>     /* srand, rand */

#pragma GCC diagnostic ignored "-Wunused-result"

int main()
{
  srand(time(NULL));

  int guess;
  int guesses_left;
  int random = rand() % 100 + 1;

  printf("Guess a number from 1-10.\n");
  for (guesses_left = 6; guesses_left > 0; guesses_left--) {
    scanf("%d", &guess);
    if (guess < random) {
      printf("Too low...\n");
    }
    else if (guess > random) {
      printf("Too high...\n");
    }
    else {
      printf("You win!\n");
      return 0;
    }
  }
  printf("You're out of guesses!\n");
}
