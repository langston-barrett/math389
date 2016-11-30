#include <stdint.h>
#include <stdio.h>
#include "./minunit.h"
#include "./solitaire-lib.c"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_char_to_card() {
  // identity under the face array indices
  for (uint8_t i = 0; i < 13; i++)
    assert_int64_eq(i, char_to_card(face[i][0], 0, 0, NULL)->face);
  // intentional error
  /* massert(char_to_card('Z', 0, 0, NULL) == NULL); */
  return 0;
}

static char *test_arena_to_str() {
  arena_t *arena = newArena();
  char *arena_str = arena_to_str(arena);
  massert(arena != NULL);
  massert(arena_str != NULL);
  assert_str_eq("10000", arena_str);
  return 0;
}

static char *test_str_to_arena() {
  // empty arena
  arena_t *arena = str_to_arena("10000", 0); // 1 player, empty
  massert(arena != NULL);
  massert(arena->players == 1);
  massert(arena->suits != NULL);
  for (int8_t i = 0; i < 4; i++) massert(arena->suits[i] != NULL);
  for (int8_t i = 0; i < 4; i++) massert(arena->suits[i]->top == NULL);

  // single-player arena
  arena_t *arena2 = str_to_arena("1AJK9", 0); // 1 player
  massert(arena2 != NULL);
  massert(arena2->players == 1);
  massert(arena2->suits != NULL);
  for (int8_t i = 0; i < 4; i++) 
    massert(arena2->suits[i] != NULL);
  /* for (int8_t i = 0; i < 4; i++) if (arena2->suits[i]->top == NULL) printf("%d\n", i); */
  for (int8_t i = 0; i < 4; i++) massert(arena2->suits[i]->top != NULL);

  // double-player arena
  arena_t *arena3 = str_to_arena("2A0K9820T", 0); // 2 players
  massert(arena3 != NULL);
  massert(arena3->players == 2);
  massert(arena3->suits != NULL);
  for (int8_t i = 0; i < 8; i++) massert(arena3->suits[i] != NULL);

  // debug
  /* for (int8_t i = 0; i < 8; i++) */
  /*   if (arena3->suits[i]->top != NULL) */
  /*     putCard(arena3->suits[i]->top); */

  massert(arena3->suits[0]->top != NULL);    // A
  massert(arena3->suits[0]->top->face == 1); // A
  massert(arena3->suits[1]->top == NULL); // 0
  massert(arena3->suits[2]->top != NULL);     // K
  massert(arena3->suits[2]->top->face == 13); // K

  return 0;
}

static char *all_tests() {
  run_test(test_char_to_card);
  run_test(test_arena_to_str);
  run_test(test_str_to_arena);
  return 0;
}

int main(void) { test_main(all_tests); }
