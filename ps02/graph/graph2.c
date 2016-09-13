#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "myMath.h"
#include "charLines.h"

void print_2d_array(char **arr, int width, int height) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      printf("%c", arr[y][x]);
    }
    printf("%c", '\n');
  }
  printf("%c", '\n');
}

// make a graph, fill it with " "
char **zero_graph(int width, int height) {
  char **g = malloc(height * sizeof(NULL)); // g[i] is a row
  for (int y = 0; y < height; y++) {
    g[y] = malloc(width * sizeof(char));
    for (int x = 0; x < width; x++) {
      g[y][x] = ' ';
    }
  }
  return g;
}

// write axes down the middle x, y
char **axes(char **g, int width, int height) {
  int middle_x = width / 2;
  int middle_y = height / 2;
  for (int x = 0; x < width; x++) {
    g[middle_y][x] = '-';
  }

  for (int y = 0; y < height; y++) {
    g[y][middle_x] = '|';
  }
  return g;
}

// actually graph the sin function
char **graph_sin(char **g, int width, int height, double x_scale, double y_scale, int power) {
  // shift x before putting it into power(sin(x))
  double x_offset = ((double) width) / 2.0;
  // shift y after rounding it
  int y_offset = height / 2;
  for (int x = 0; x < width; x++) {
    double adjusted_x = (((double) x) - x_offset) * x_scale;
    double raw_y = powerOf(sin(adjusted_x), power);
    int y = round((-raw_y) * y_scale) + y_offset;
    if (y >= 0 && y < height) {
      g[y][x] = '*';
    }
  }
  return g;
}

char **graph(int w, int h, int power) {
  return graph_sin(axes(zero_graph(w, h), w, h), w, h, .2, 2, power);
}

int main() {
  int width = 65;
  int height = 20;
  int power;

  printf("Enter an integer power: ");
  if (scanf("%d",&power)) {
    print_2d_array(graph(width, height, power), width, height);
    return 0;
  }
  printf("There was an error\n");
  return 1;
}
