#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "myMath.h"
#include "charLines.h"

// To compile:
//
//   gcc -c myMath.c
//   gcc -c charLines.c
//   gcc -c graph.c
//   gcc -o graph -lm myMath.o charLines.o graph.o
//
// or just
//
//   gcc -o graph graph.c charLines.c myMath.c -lm
//
// or just
//
//   make graph
//

double xscale = 3.14159;
double yscale = 1.2;
int height = 20;

void out(char *s) {
  printf("%s",s);
}

void oneLine(double y, bool yaxis) {

  int h;
  char *fill = " ";
  char *cntr = "|";


  if (yaxis) {
    fill = "-";
    cntr = "+";
  }

  h = (int)round(fabs(y)*(double)height/yscale);

  if (h == 0) {
    out(" ");
    repeat(fill,height);
    out("*");
    repeat(fill,height);
  } else if (y > 0.0) {
    out(" ");
    repeat(fill,height);
    out(cntr);
    if (h < height) {
      repeat(fill,h-1);
      out("*");
    } else {
      repeat(fill,height);
      out(">");
    }
  } else {
    if (h < height) {
      out(" ");
      repeat(fill,height-h);
      out("*");
      repeat(fill,h-1);
    } else {
      out("<");
      repeat(fill,height);
    }
    out(cntr);
  }
}

int main() {
  double x;
  int line;
  int p;

  printf("Enter an integer power: ");
  scanf("%d",&p);

  for (line = -height+1; line < height; line++) {
    x = (xscale*(double)line)/(double)height;
    oneLine(powerOf(sin(x),p),line==0);
    out("\n");
  }

}
