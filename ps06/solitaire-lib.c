/******************************* IMPORTS *******************************/
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/******************************* CONSTANTS *******************************/

#ifndef DEBUG
#define DEBUG 1
#endif

void srand48(long);
double drand48(void);

#define SPADES 0
#define HEARTS 1
#define CLUBS 2
#define DIAMONDS 3

#define DRAW 4
#define DISCARD 5
#define LAIN(pile) (5 + (pile))
#define HIDDEN(pile) (12 + (pile))

#define BLACK 0
#define RED 1

char red[] = {0x1b, 0x5b, 0x33, 0x31, 0x6d, 0};
char black[] = {0x1b, 0x5b, 0x33, 0x30, 0x6d, 0};
char neutral[] = {0x1b, 0x5b, 0x33, 0x39, 0x6d, 0};
char whitebg[] = {0x1b, 0x5b, 0x33, 0x47, 0x6d, 0};
char neutralbg[] = {0x1b, 0x5b, 0x33, 0x49, 0x6d, 0};

char spades[] = {0xE2, 0x99, 0xA0, 0};
char hearts[] = {0xE2, 0x99, 0xA5, 0};
char clubs[] = {0xE2, 0x99, 0xA3, 0};
char diamonds[] = {0xE2, 0x99, 0xA6, 0};

char *suit[] = {spades, hearts, clubs, diamonds};
char *suitstr[] = {"S", "H", "C", "D"};
char *face[] = {"O", "A", "2", "3", "4", "5", "6",
                "7", "8", "9", "T", "J", "Q", "K"};

/******************************* TYPES *******************************/

typedef struct _card_t {
  uint8_t player;
  uint8_t face;
  uint8_t suit;
  struct _card_t *below;
  struct _stAck_t *stack;
} card_t;

typedef struct _deck_t {
  int player;
  card_t cards[52];
} deck_t;

typedef struct _stAck_t {
  card_t *top;
  int type;
} stAck_t;

// a node in the arena's linked list
typedef struct _arena_node_t {
  stAck_t *stack;
  struct _arena_node_t *next;
} arena_node_t;

// the size of an arena is 4 * # players
// the order of suits is: spades hearts clubs diamonds, just like in suit[].
typedef struct _arena_t {
  uint8_t players; // should never be more than 9
  uint8_t id;      // unused: for when the server is multi-game
  arena_node_t *first;
  pthread_mutex_t *lock;
} arena_t;

typedef struct _solitaire_t {
  int player;
  deck_t *deck;
  stAck_t *hidden[8];
  stAck_t *lain[8];
  stAck_t *discard;
  stAck_t *draw;
} solitaire_t;

stAck_t *newStack(int type) {
  stAck_t *stack = malloc(sizeof(stAck_t));
  assert(stack != NULL);
  stack->top = NULL;
  stack->type = type;
  return stack;
}

/******************************* PREDICATES *******************************/

int isRed(card_t *c) {
  assert(c != NULL);
  return c->suit % 2 == RED;
}

int isBlack(card_t *c) {
  assert(c != NULL);
  return c->suit % 2 == BLACK;
}

int isAce(card_t *c) {
  assert(c != NULL);
  return c->face == 1;
}

int isKing(card_t *c) {
  assert(c != NULL);
  return c->face == 13;
}

int isTop(card_t *c) {
  assert(c != NULL);
  return c->stack->top == c;
}

int pileOf(stAck_t *stack) {
  assert(stack != NULL);
  return stack->type - LAIN(1) + 1;
}

int isLain(stAck_t *stack) {
  assert(stack != NULL);
  return (LAIN(1) <= stack->type) && (stack->type <= LAIN(7));
}

int isUp(card_t *c) {
  assert(c != NULL);
  return isLain(c->stack) || (isTop(c) && (c->stack->type == DISCARD));
}

int okOn(card_t *c1, card_t *c2) {
  assert(c1 != NULL);
  assert(c2 != NULL);
  return ((isRed(c1) && isBlack(c2)) || (isRed(c2) && isBlack(c1))) &&
         (c1->face + 1 == c2->face);
}

/******************************* STACKS *******************************/
int isEmpty(stAck_t *stack) { return stack->top == NULL; }

void push(card_t *card, stAck_t *stack) {
  assert(card != NULL);
  assert(stack != NULL);

  card->below = stack->top;
  stack->top = card;
  card->stack = stack;

  assert(card != NULL);
  assert(stack != NULL);
}

card_t *pop(stAck_t *stack) {
  assert(stack != NULL);
  assert(stack->top != NULL);

  card_t *card = stack->top;
  stack->top = card->below;
  card->below = NULL;
  card->stack = NULL;

  assert(card != NULL);
  assert(stack != NULL);
  return card;
}

/******************************* ENCODING *******************************/

// create a lock and handle errors
pthread_mutex_t *new_lock() {
  pthread_mutex_t *lock = malloc(sizeof(pthread_mutex_t));
  if (lock == NULL)
    printf("[FATAL] Out of memory\n");
  if (pthread_mutex_init(lock, NULL) != 0)
    printf("[FATAL] Can't create the lock");
  return lock;
}

char *card_to_str(card_t *card) {
  char *to_return = malloc(3 * sizeof(char));
  sprintf(to_return, "%c%c", face[card->face][0], suitstr[card->suit][0]);
  return to_return;
}

card_t *str_to_card(char *card_str) {
  // construct the initial values
  card_t *to_return = malloc(sizeof(card_t));
  assert(to_return != NULL);
  to_return->face = 100;
  to_return->suit = 100;

  for (uint8_t i = 0; i < 14; i++)
    if (card_str[0] == face[i][0])
      to_return->face = i;

  for (uint8_t i = 0; i < 4; i++)
    if (card_str[1] == suitstr[i][0])
      to_return->suit = i;

  // reject ill-formed descriptions
  assert(to_return->face != 100);
  assert(to_return->suit != 100);
  return to_return;
}

// encode the arena as a string
// format: #players stack1_top stack2_top ...
// where there are 4 * # players stacks and no spaces in between each token
char *arena_to_str(arena_t *arena) {
  assert(arena != NULL);

  char *arena_str = malloc(80 * sizeof(char));
  if (arena_str == NULL)
    printf("[ERROR] null arena_str\n");
  arena_str[0] = '\000'; // empty the string out

  // first, send the current number of players
  char *players_str = malloc(20 * sizeof(char));
  sprintf(players_str, "%d", arena->players);
  strcat(arena_str, players_str);
  assert(arena_str != NULL);

  // now, append the cards in each stack
  arena_node_t *current = arena->first;
  while (current != NULL) {
    assert(current->stack != NULL);
    assert(current->stack->top != NULL);
    printf("%s\n", arena_str);
    strcat(arena_str, card_to_str(current->stack->top));
    current = current->next;
  }
  assert(arena_str != NULL);
  return arena_str;
}

arena_t *str_to_arena(char *arena_str, uint8_t player_id) {
  arena_t *arena = malloc(sizeof(arena_t));

  // TODO: check that it's a valid int
  arena->players = (uint8_t)arena_str[0] - '0'; // ASCII char->int
  arena->lock = new_lock();

  // for each 2-length card description
  for (uint8_t i = 1; i < strlen(arena_str); i = i + 2) {
    arena_node_t *new_node = malloc(sizeof(arena_node_t));

    // extract the card string, convert it to a card
    char *cardstr = malloc(4 * sizeof(char));
    sprintf(cardstr, "%c%c", arena_str[i], arena_str[i + 1]);
    card_t *card = str_to_card(cardstr);

    // add a stack with that card on top
    new_node->stack = newStack(0);
    push(card, new_node->stack);

    // push it onto the arena
    if (arena->first == NULL) // prepend
      arena->first = new_node;
    else { // append
      arena_node_t *current = arena->first;
      while (current->next != NULL)
        current = current->next;
      current->next = new_node;
    }
  }

  return arena;
}

/******************************* PRINTING *******************************/

void putRed() { printf("%s", red); }
void putBlack() { printf("%s", black); }
void putBack() { printf("%s", neutral); }

void putColorOfSuit(int s) {
  if (s % 2 == RED) {
    putRed();
  } else {
    putBlack();
  }
}

void putSuit(int s) { printf("%s", suit[s]); }

void putFace(int f) { printf("%s", face[f]); }

void putCard(card_t *c) {
  assert(c != NULL);

  if (isRed(c)) {
    putRed();
  } else {
    putBlack();
  }
  putFace(c->face);
  putSuit(c->suit);
  putBack();
}

void putStack(stAck_t *stack) {
  assert(stack != NULL);

  card_t *c = stack->top;
  while (c != NULL) {
    putCard(c);
    printf(" ");
    c = c->below;
  }
}

void putArena(arena_t *arena) {
  assert(arena != NULL);

  printf("Number of players: %d\n", arena->players);
  arena_node_t *current = arena->first;
  while (current != NULL) {
    assert(current->stack != NULL);
    assert(current->stack->top != NULL);
    putColorOfSuit(current->stack->top->suit);
    putSuit(current->stack->top->suit);
    putBack();
    printf(": ");
    putStack(current->stack);
    printf("\n");

    // continue onward
    assert(current != current->next);
    current = current->next;
  }
}

void putSolitaire(solitaire_t *S) {
  assert(S != NULL);

  for (int p = 1; p <= 7; p++) {
    printf("%d: ", p);
    putStack(S->lain[p]);
    printf("[");
    putStack(S->hidden[p]);
    printf("]\n");
  }
  printf("\n[");
  putStack(S->draw);
  printf("]\n");
  putStack(S->discard);
  printf("\n");
}

void putHelp(char *extra_message) {
  printf("[ERROR] %s\n", extra_message);
  printf("[INFO] Valid commands are: 'p', 'm', 'n'.\n");
  printf("[INFO] \tp <card>: play <card> into the arena\n");
  printf("[INFO] \tm <c1> <c2>: move card 1 onto card 2\n");
  printf("[INFO] \tn: draw the next card from the deck\n");
  printf("[INFO] \tu: get an updated arena from the server\n");
  printf("[INFO] \tq: exit\n");
  printf("[INFO] Cards are specified as follows <card><suit>\n");
  printf("[INFO] where <card> is one of: 23456789tjqka \n");
  printf("[INFO] where <suit> is one of: \n");
  printf("[INFO] \ts: spades\n");
  printf("[INFO] \th: hearts\n");
  printf("[INFO] \tc: clubs\n");
  printf("[INFO] \td: diamonds.\n");
}

/******************************* GAMEPLAY *******************************/

card_t *cardOf(char *c, solitaire_t *S) {
  int i;
  if (c[0] >= '2' && c[0] <= '9') {
    i = c[0] - '0';
  } else if (c[0] == 'T' || c[0] == 't') {
    i = 10;
  } else if (c[0] == 'J' || c[0] == 'j') {
    i = 11;
  } else if (c[0] == 'Q' || c[0] == 'q') {
    i = 12;
  } else if (c[0] == 'K' || c[0] == 'k') {
    i = 13;
  } else if (c[0] == 'A' || c[0] == 'a') {
    i = 1;
  } else {
    return NULL;
  }
  int j;
  if (c[1] == 'S' || c[1] == 's') {
    j = SPADES;
  } else if (c[1] == 'H' || c[1] == 'h') {
    j = HEARTS;
  } else if (c[1] == 'C' || c[1] == 'c') {
    j = CLUBS;
  } else if (c[1] == 'D' || c[1] == 'd') {
    j = DIAMONDS;
  } else {
    return NULL;
  }
  return &S->deck->cards[j * 13 + i - 1];
}

deck_t *newDeck() {
  // Make a fresh deck of cards.
  deck_t *deck = malloc(sizeof(deck_t));
  int i, s;
  for (i = 0, s = 0; s <= 3; s++) {
    for (int f = 1; f <= 13; f++, i++) {
      deck->cards[i].suit = s;
      deck->cards[i].face = f;
      deck->cards[i].below = NULL;
      deck->cards[i].stack = NULL;
    }
  }
  return deck;
}

void shuffleInto(deck_t *deck, stAck_t *draw) {
  assert(deck != NULL);
  assert(draw != NULL);

  card_t *shuffle[52];

  // Get a reference to each of the 52 cards.
  for (int i = 0; i < 52; i++) {
    shuffle[i] = &deck->cards[i];
  }

  // Perform a Knuth shuffle.
  for (int i = 0; i < 52; i++) {

    // Select the next card.
    int r = (int)((52 - i) * drand48());
    if (r != 0) {
      card_t *tmp = shuffle[i];
      shuffle[i] = shuffle[i + r];
      shuffle[i + r] = tmp;
    }

    // Put it onto the draw pile.
    push(shuffle[i], draw);
  }
}

arena_t *newArena(void) {
  arena_t *arena = malloc(sizeof(arena_t));
  arena->players = 0;
  arena->first = NULL;
  arena->lock = new_lock();
  return arena;
}

solitaire_t *newSolitaire(long seed) {
  srand48(seed);

  solitaire_t *S = (solitaire_t *)malloc(sizeof(solitaire_t));
  S->deck = newDeck();

  // Make all the stacks.
  S->draw = newStack(DRAW);
  S->discard = newStack(DISCARD);
  for (int p = 1; p <= 7; p++) {
    S->lain[p] = newStack(LAIN(p));
    S->hidden[p] = newStack(HIDDEN(p));
  }

  // Build the shuffled draw pile.
  shuffleInto(S->deck, S->draw);

  // Make each of the hidden piles.
  for (int i = 1; i <= 7; i++) {
    for (int j = i; j <= 7; j++) {
      push(pop(S->draw), S->hidden[j]);
    }
  }

  // Flip the top card of each pile.
  for (int i = 1; i <= 7; i++) {
    push(pop(S->hidden[i]), S->lain[i]);
  }

  // Flip up the first card.
  push(pop(S->draw), S->discard);

  return S;
}

void maybeFlip(stAck_t *stack, solitaire_t *S) {
  assert(stack != NULL);
  assert(S != NULL);

  if (isLain(stack) && isEmpty(stack)) {
    int p = pileOf(stack);
    if (!isEmpty(S->hidden[p])) {
      push(pop(S->hidden[p]), S->lain[p]);
    }
  }
}

// Can I play this card on that one in the arena?
bool can_play_on(card_t *card, card_t *arena_card) {
  assert(arena_card != NULL);
  assert(card != NULL);
  return (arena_card->suit == card->suit &&
         (arena_card->face == card->face + 1 || // either it's one lower
          (arena_card->face == 1 && card->face == 13))); // or it's ace/king pair
}

char *play(card_t *card, arena_t *arena, solitaire_t *S) {
  assert(card != NULL);
  assert(S != NULL);
  assert(card->stack != NULL);
  assert(arena != NULL);
  assert(arena->lock != NULL);

  // validate all the stacks
  arena_node_t *current = arena->first;
  while (current != NULL) {
    assert(current->stack != NULL);
    assert(current->stack->top != NULL);
    current = current->next;
  }

  if (!isTop(card))
    return "that card is not at the top of its stack";

  // we have to lock the arena for this bit
  pthread_mutex_lock(arena->lock); // LOCK

  current = arena->first;
  if (card->face == 1) { // if it's an ace, append it
    // set up our new node
    arena_node_t *new_node = malloc(sizeof(arena_node_t));
    new_node->stack = newStack(0);
    push(pop(card->stack), new_node->stack);

    if (current == NULL) { // insert at the front
      arena->first = new_node;
      pthread_mutex_unlock(arena->lock); // UNLOCK
      return NULL;
    }
    // insert at the end
    while (current->next != NULL)
      current = current->next;
    current->next = new_node;
    pthread_mutex_unlock(arena->lock); // UNLOCK
    return NULL;
  } else { // otherwise, try and find a suitable stack to put it on
    while (current != NULL) {
      card_t *stack_card = current->stack->top;

      printf("card->suit: %d\n", card->suit);
      printf("stack_card->suit: %d\n", stack_card->suit);
      printf("card->face: %d\n", card->face);
      printf("stack_card->face: %d\n", stack_card->face);

      // if we can actually lay it down...
      if (can_play_on(card, stack_card)) {
        printf("in condition\n");
        push(pop(card->stack), current->stack);
        maybeFlip(card->stack, S);
        pthread_mutex_unlock(arena->lock); // UNLOCK
        return NULL;
      }
      current = current->next;
    }
  }
  printf("returning bad\n");
  pthread_mutex_unlock(arena->lock); // UNLOCK
  return "couldn't find an appropriate stack";
}

void move(card_t *card, stAck_t *dest, solitaire_t *S) {
  stAck_t *srce = card->stack;
  card_t *top = srce->top;
  card_t *c = top;
  while (c != card) {
    c->stack = dest;
    c = c->below;
  }
  card->stack = dest;
  srce->top = card->below;
  card->below = dest->top;
  dest->top = top;
  maybeFlip(srce, S);
}

int isBottom(char *cs) { return strcmp(cs, "B") == 0; }

stAck_t *freeLain(solitaire_t *S) {
  for (int p = 1; p <= 7; p++) {
    if (isEmpty(S->lain[p])) {
      return S->lain[p];
    }
  }
  return NULL;
}

bool moveOnto(card_t *card, card_t *onto, solitaire_t *S) {
  assert(card != NULL);
  assert(onto != NULL);
  assert(S != NULL);

  if (onto == NULL) {

    if (isKing(card)) {
      stAck_t *dest = freeLain(S);
      if (isUp(card) && dest != NULL) {
        move(card, dest, S);
        return true;
      }
    }

  } else {

    stAck_t *dest = onto->stack;
    if (isLain(dest) && isUp(card) && isTop(onto) && okOn(card, onto)) {
      move(card, dest, S);
      return true;
    }
  }

  return false;
}

// validate a command string, returning the appropriate error if necessary.
// Returns error string or NULL if there wasn't an error
char *validate_command(char *cmd, solitaire_t *S) {
  assert(cmd != NULL);
  assert(S != NULL);

  printf("[DEBUG] validating: %s", cmd);

  char trash[80];
  char c1[80];
  char c2[80];

  if (cmd[0] == 'q') {
    return NULL;
  }
  if (cmd[0] == 'p') {
    sscanf(cmd, "%s %s", trash, c1);
    if (cardOf(c1, S) == NULL)
      return "no such card";

  } else if (cmd[0] == 'm') {
    sscanf(cmd, "%s %s %s", trash, c1, c2);
    card_t *card1 = cardOf(c1, S);
    card_t *card2 = cardOf(c2, S);

    if (card1 == NULL)
      return "no such card";
    else if (card2 == NULL)
      return "no such card";
    else if (!okOn(card1, card2))
      return "can't play card1 on card2";

  } else if (cmd[0] == 'n') {
    if (isEmpty(S->draw))
      return "can't draw: empty deck";
  } else
    return "invalid command, valid commands are 'p', 'm', 'n'";

  printf("[DEBUG] valid!\n");
  return NULL;
}

// Update the game state according to cmd
// Returns error string or NULL if there wasn't an error
char *update(char *cmd, arena_t *A, solitaire_t *S) {
  assert(cmd != NULL);
  assert(A != NULL);
  assert(S != NULL);

  char trash[80];
  char c1[80];
  char c2[80];

  if (cmd[0] == 'p') { // Play a card on top of some stack to the arena.
    sscanf(cmd, "%s %s", trash, c1);

    card_t *card = cardOf(c1, S);
    assert(card != NULL); // card != null validated in validate_command

    return play(card, A, S);

  } else if (cmd[0] == 'm') { // Move a card to some lain stack.
    sscanf(cmd, "%s %s %s", trash, c1, c2);
    // card != null validated in validate_command
    if (moveOnto(cardOf(c1, S), cardOf(c2, S), S)) {
      return NULL;
    } else {
      return "that move cannot be made";
    }
  } else if (cmd[0] == 'n') { // Draws the next card and puts it on top of the
                              // discard pile.
    // deck != empty validated above
    push(pop(S->draw), S->discard);
  } else {
    return "command validation let an invalid one slip";
  }

  return NULL;
}

/******************************* NETWORKING *******************************/

// read a little!
char *read_some(int connfd) {
  char *feedback = malloc(80 * sizeof(char));
  int n = read(connfd, feedback, 80);
  if (n == 0) {
    // No bytes received. The other end closed the connection.
    return NULL;
  }
  return feedback;
}

// send a message, expecting a reply. returns NULL when the other end has closed
// the connection, or on error.
char *return_service_requested(int connfd, char *msg) {
  assert(msg != NULL);

  if (DEBUG)
    printf("[DEBUG] Sending message: %s\n", msg);

  if (!write(connfd, msg, strlen(msg) + 1)) {
    printf("[ERROR] Write failed.\n");
    return NULL;
  }

  if (DEBUG)
    printf("[DEBUG] Awaiting response...\n");
  return read_some(connfd);
}
