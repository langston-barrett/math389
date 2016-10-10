#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
   @brief Turn two arrays of ints into a valid CSV file.
   @param arr1 The first array
   @param arr2 The second array
   @param size The lengths of the arrays (they must be the same length).
   @return The string representation of the CSV
*/
char *csv(double *arr1, int64_t *arr2, int64_t size) {
  // log_10(2^64) < 20
  // each line contains two int64s, one comma, and one newline
  //                 i     ,   j    \n
  int64_t line_size = 20 + 1 + 20 + 1;
  char *to_return = malloc(line_size * size * sizeof(char));
  char line[line_size];
  for (int64_t i = 0; i < size; i++) {
    sprintf(line, "%f,%li\n", arr1[i], arr2[i]);
    strcat(to_return, line);
  }
  return to_return;
}

/**
   @brief Write a string to a file.
   @param filepath Self-explanatory.
   @param data The string to write.
   @return A boolean indicating success or failure.
*/
bool str_to_file(char *filepath, char *data) {
  FILE *file_out = fopen(filepath, "w");
  bool to_return = false;
  if (file_out != NULL) {
    fputs(data, file_out);
    // "On error, the function returns EOF and sets the error indicator"
    if (fputs (data, file_out) != EOF) {
      to_return = true;
    }
    fclose(file_out);
  }
  return to_return;
}
