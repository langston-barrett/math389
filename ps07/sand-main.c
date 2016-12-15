#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "./sand.h"
#include "./tsand.h"
#include "./sand-lib.h"

#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"

#define MAX_HEIGHT 4

// just parse the command line args.
static uint16_t __attribute__((nonnull))
parse_arg(char *str, bool odd, char *err_str) {
  uint16_t parsed = (uint16_t)strtoul(str, NULL, 10);
  if (parsed == 0 || (odd && parsed % 2 == 0) || errno != 0) {
    fprintf(stderr, "[ERROR] invalid %s: '%s'\n", err_str, str);
    exit(1);
  }
  return parsed;
}

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "[ERROR] usage: %s <rows> <columns> <center-height>\n",
            argv[0]);
    return 1;
  }
  uint16_t rows = parse_arg(argv[1], true, "number of rows");
  uint16_t cols = parse_arg(argv[2], true, "number of columns");
  uint16_t center = parse_arg(argv[3], false, "center height");
  pile_ *pile = new_pile(rows, cols, center, MAX_HEIGHT);

#ifdef THREADS
  print_pile(tevolve(pile), true);
#endif
#ifndef THREADS
  print_pile(evolve(pile, true, true), true);
#endif

  return 0;
}
