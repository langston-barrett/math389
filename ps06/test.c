#include <stdint.h>
#include <stdio.h>
#include "./minunit.h"
#include "./solitaire-lib.c"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

static char *test_play() {
  solitaire_t *solitaire = newSolitaire(0);
  arena_t *arena = newArena();

  // play all four suits, in order
  for (uint8_t suit_index = 0; suit_index < 3; suit_index++) {
    for (uint8_t face_index = 1; face_index < 14; face_index++) {
      card_t *card = malloc(sizeof(card_t));
      card->face = face_index;
      card->suit = 2;
      stAck_t *stack = newStack(0);
      push(card, stack);

      massert(play(card, arena, solitaire));

      // the lock should be unlocked
      massert(pthread_mutex_trylock(arena->lock) == 0);
      pthread_mutex_unlock(arena->lock);
    }
  }

  // play a bunch of aces
  card_t *card = str_to_card("AD");
  stAck_t *stack = newStack(0);
  push(card, stack);
  for (uint8_t i = 0; i < 100; i++)
    massert(play(card, arena, solitaire));
  return 0;
}

static char *test_str_to_card() {
  massert(str_to_card("KH") != NULL);
  assert_int64_eq(1, str_to_card("AS")->face);
  assert_int64_eq(1, str_to_card("2H")->suit);
  assert_int64_eq(2, str_to_card("TC")->suit);
  assert_int64_eq(10, str_to_card("TD")->face);
  assert_int64_eq(11, str_to_card("JD")->face);
  return 0;
}

static char *test_card_to_str() {
  assert_str_eq("AS", card_to_str(str_to_card("AS")));
  assert_str_eq("TH", card_to_str(str_to_card("TH")));
  return 0;
}

static char *test_card_encoding_identity() {
  for (uint8_t suit_index = 0; suit_index < 4; suit_index++) {
    for (uint8_t face_index = 0; face_index < 14; face_index++) {
      card_t *original = malloc(sizeof(card_t));
      original->suit = suit_index;
      original->face = face_index;
      char *card_str = card_to_str(original);
      card_t *decoded = str_to_card(card_str);
      // they should be the same card
      assert_int64_eq(original->suit, decoded->suit);
      assert_int64_eq(original->face, decoded->face);
    }
  }
  return 0;
}

static char *test_arena_to_str() {
  arena_t *arena = newArena();
  massert(arena != NULL);
  massert(arena_to_str(arena) != NULL);
  assert_str_eq("0", arena_to_str(arena));

  /* arena_t *arena2 = newArena(); */
  /* solitaire_t *solitaire = newSolitaire(0); */
  /* play(str_to_card("AS"), arena2, solitaire); */
  /* assert_str_eq("0", arena_to_str(arena)); */
  return 0;
}

/* static char *test_str_to_arena() { */
/*   // empty arena */
/*   arena_t *arena = str_to_arena("1", 0); // 1 player, empty */
/*   massert(arena != NULL); */
/*   massert(arena->players == 1); */
/*   massert(arena->first == NULL); */

/*   arena_t *arena2 = str_to_arena("1AJK9", 0); // 1 player, full */
/*   massert(arena2 != NULL); */
/*   massert(arena2->players == 1); */
/*   massert(arena2->first != NULL); */
/*   // TODO stronger */

/*   arena_t *arena3 = str_to_arena("2A0K9820T", 0); // 2 players, full */
/*   massert(arena3 != NULL); */
/*   massert(arena3->players == 2); */
/*   massert(arena3->first != NULL); */
/*   // TODO stronger */

/*   return 0; */

static char *all_tests() {
  run_test(test_play);
  run_test(test_str_to_card);
  run_test(test_card_to_str);
  run_test(test_card_encoding_identity);
  run_test(test_arena_to_str);
  /* run_test(test_str_to_arena); */
  return 0;
}

int main(void) { test_main(all_tests); }
