#ifndef _sand_h
#define _sand_h
#include <stdbool.h>
#include <stdint.h>
#include "./sand-lib.gen.h"
bool step(pile_ *src, pile_ *dst);
pile_ *evolve(pile_ *src, bool print, bool color);
#endif
