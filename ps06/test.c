#include <stdint.h>
#include <stdio.h>
#include "./minunit.h"
#include "./solitaire-lib.c"

static uint64_t tests_run = 0;
static uint64_t tests_failed = 0;
static char message[1024];

// makes a new card with the appropriate suit & face + a new stack
card_t *new_card(uint8_t suit, uint8_t face, stAck_t *stack) {
  assert(face <= 13);
  assert(suit < 4);
  card_t *card = malloc(sizeof(card_t));
  card->face = face;
  card->suit = suit;
  if (stack == NULL) {
    stAck_t *new_stack = newStack(0);
    push(card, new_stack);
  } else {
    push(card, stack);
  }
  return card;
}

/******************************* ENCODING *******************************/
// These are tested first because it's nice to use str_to_card in our other
// tests.

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
      card_t *original = new_card(suit_index, face_index, NULL);
      char *card_str = card_to_str(original);
      card_t *decoded = str_to_card(card_str);
      // they should be the same card
      assert_int64_eq(original->suit, decoded->suit);
      assert_int64_eq(original->face, decoded->face);
    }
  }
  return 0;
}

/******************************* STACKS *******************************/

// push every card onto a stack
stAck_t *full_stack(uint8_t suit_index) {
  stAck_t *stack = newStack(0);
  for (uint8_t face_index = 1; face_index < 14; face_index++)
    new_card(suit_index, face_index, stack);
  return stack;
}

static char *test_push() {
  for (uint8_t suit_index = 0; suit_index < 4; suit_index++)
    massert(full_stack(suit_index) != NULL);
  return 0;
}

static char *test_pop() {
  for (uint8_t suit_index = 0; suit_index < 4; suit_index++) {
    stAck_t *stack = full_stack(suit_index);
    for (uint8_t face_index = 13; face_index > 0; face_index--) {
      massert(stack != NULL);
      massert(stack->top != NULL);
      card_t *card = pop(stack);
      massert(card != NULL);
      massert(card->suit == suit_index);
      massert(card->face == face_index);
    }
  }
  return 0;
}

/******************************* ENCODING *******************************/

static char* test_can_play_on() {
  massert(can_play_on(str_to_card("KD"), str_to_card("AD"))); // special case
  massert(can_play_on(str_to_card("2C"), str_to_card("3C")));
  massert(can_play_on(str_to_card("TH"), str_to_card("JH")));
}

static char *test_play() {
  // basic: push a king on top of an ace
  solitaire_t *solitaire2 = newSolitaire(0);
  arena_t *arena2 = newArena();
  card_t *ad = new_card(1, 1, NULL); // AD
  card_t *kd = new_card(1, 13, NULL); // KD
  massert(play(ad, arena2, solitaire2) == NULL);
  massert(play(kd, arena2, solitaire2) == NULL);

  // more advanced, automated tests
  solitaire_t *solitaire = newSolitaire(0);
  arena_t *arena = newArena();

  // play all four suits, in order
  for (uint8_t suit_index = 0; suit_index < 4; suit_index++) {
    // play ace
    card_t *ace = new_card(suit_index, 1, NULL);
    massert(play(ace, arena, solitaire) == NULL);

    // descend down from kings -> twos
    for (uint8_t face_index = 13; face_index > 1; face_index--) {
      card_t *card = new_card(face_index, suit_index, NULL);
      massert(play(card, arena, solitaire) == NULL);

      // the lock should be unlocked
      massert(pthread_mutex_trylock(arena->lock) == 0);
      pthread_mutex_unlock(arena->lock);
    }
  }

  // can't play anything on top of full stacks that isn't an ace
  for (uint8_t suit_index = 0; suit_index < 4; suit_index++) {
    for (uint8_t face_index = 2; face_index < 14; face_index++) {
      card_t *card = new_card(face_index, suit_index, NULL);
      massert(play(card, arena, solitaire) != NULL);

      // the lock should be unlocked
      massert(pthread_mutex_trylock(arena->lock) == 0);
      pthread_mutex_unlock(arena->lock);
    }
  }

  // play a bunch of aces
  card_t *a = new_card(2, 1, NULL); // some ace
  for (uint8_t i = 0; i < 100; i++)
    massert(play(a, arena, solitaire) == NULL);
  return 0;
}

static char *test_arena_to_str() {
  arena_t *arena = newArena();
  massert(arena != NULL);
  massert(arena_to_str(arena) != NULL);
  assert_str_eq("0", arena_to_str(arena));

  arena_t *arena2 = newArena();
  solitaire_t *solitaire = newSolitaire(0);
  card_t *as = new_card(0, 1, NULL); // AS
  play(as, arena2, solitaire);
  assert_str_eq("0AS", arena_to_str(arena2));

  return 0;
}

static char *test_str_to_arena() {
  // empty arena
  arena_t *arena = str_to_arena("1", 0); // 1 player, empty
  massert(arena != NULL);
  massert(arena->players == 1);
  massert(arena->first == NULL);

  arena_t *arena2 = str_to_arena("1ASJHKD9C", 0); // 1 player, full
  massert(arena2 != NULL);
  massert(arena2->players == 1);
  massert(arena2->first != NULL);
  // TODO stronger

  arena_t *arena3 = str_to_arena("2ASKH9C8D2STS", 0); // 2 players, partially full
  massert(arena3 != NULL);
  massert(arena3->players == 2);
  massert(arena3->first != NULL);
  // TODO stronger

  return 0;
}


static char *all_tests() {
  run_test(test_push);
  run_test(test_pop);

  run_test(test_can_play_on);
  run_test(test_play);

  run_test(test_str_to_card);
  run_test(test_card_to_str);
  run_test(test_card_encoding_identity);
  run_test(test_arena_to_str);
  run_test(test_str_to_arena);

  return 0;
}

int main(void) { test_main(all_tests); }
