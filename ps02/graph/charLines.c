#include "charLines.h"
#include <stdio.h>

void repeat(char *theString, int numberOfTimes) {
  int i;
  for (i=0; i<numberOfTimes; i++) {
    printf("%s",theString);
  }
}

void spaces(int number) {
  repeat(" ",number);
}
