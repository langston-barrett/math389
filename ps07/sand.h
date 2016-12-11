#ifndef __sand_h
#define __sand_h
#include <stdbool.h>
#include <stdint.h>
#include "./sand-lib.gen.h"
bool step(pile *src, pile *dst);
pile *evolve(pile *src, bool print, bool color);
#endif
