#include <stdio.h>
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wunused-result"

int main(int argc, char **argv) {
  FILE *f_in, *f_out;
  int rows, columns;

  if (argc <= 1) {
    fprintf(stderr, "usage: %s <name of spreadsheet file>\n", argv[0]);
    return -1;
  }

  //
  // Read the input file.
  //
  f_in = fopen(argv[1],"r");
  if (f_in == 0) {
    fprintf(stderr, "Error: file '%s' not found.\n", argv[1]);
    return -1;
  }

  printf("Reading '%s'...\n", argv[1]);
  fscanf(f_in, "%d", &rows);
  fscanf(f_in, "%d", &columns);

  // fill the table with arrays
  int **table = malloc(rows * sizeof(NULL));
  for (int r = 0; r < rows; r++) {
    table[r] = malloc(columns * sizeof(int));
  }

  // create a transposed copy
  int **new_table = malloc(columns * sizeof(NULL));
  for (int c = 0; c < columns; c++) {
    new_table[c] = malloc(rows * sizeof(int));
    for (int r = 0; r < rows; r++) {
      new_table[c][r] = 0;
    }
  }

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      fscanf(f_in, "%d", &(table[r][c]));
    }
  }

  //
  // Transpose the columns
  //
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      new_table[c][r] = table[r][c];
    }
  }

  fclose(f_in);

  //
  // Write the output file.
  //
  f_out = fopen("transpose.txt","w");
  if (f_out == 0) {
    fprintf(stderr, "Error: cannot create 'transpose.txt'.\n");
    return -1;
  }
  printf("Writing '%s'...\n", "transpose.txt");
  fprintf(f_out, "%d %d\n", rows, columns);
  for (int c = 0; c < columns; c++) {
    for (int r = 0; r < rows; r++) {
      fprintf(f_out, "%d ", new_table[c][r]);
    }
    fprintf(f_out, "\n");
  }
  fclose(f_out);


  return 0;
}
