#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include "./solitaire-lib.c"

#ifndef DEBUG
#define DEBUG 1
#endif

arena_t *get_arena(int client_fd, uint8_t player_id) {
  char *arena_str = malloc(80 * sizeof(char)); // TODO: how big?
  arena_str = return_service_requested(client_fd, "u");

  // ensure we receive something from the server
  if (arena_str == NULL) {
    printf("[ERROR] No arena given by the server.\n");
    return NULL;
  }

  // ensure it was well-formed, and convert it into an arena
  arena_t *arena = str_to_arena(arena_str, player_id);
  if (arena == NULL)
    printf("[ERROR] Arena was ill-formed.\n");

  return arena;
}

// connect to server, give back socket file descriptor
int connect_to_server(char *hostname, int port) {

  // Look up the host's name to get its IP address.
  struct hostent *hp;
  if ((hp = gethostbyname(hostname)) == NULL) {
    fprintf(stderr, "[ERROR] gethostbyname(%s) failed.\n", hostname);
    exit(-1);
  }

  // Request a socket and get its file descriptor.
  int clientfd;
  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "[ERROR] Socket creation failed.\n");
    exit(-1);
  }

  // Fill in the host/port info into a (struct sockaddr_in).
  struct sockaddr_in serveraddr;
  bzero((char *)&serveraddr, sizeof(struct sockaddr_in));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr,
        hp->h_length);
  serveraddr.sin_port = htons(port);

  // Connect to the given host at the given port number.
  if (connect(clientfd, (struct sockaddr *)&serveraddr,
              sizeof(struct sockaddr_in)) < 0) {
    fprintf(stderr, "[ERROR] Connect failed.\n");
    exit(-1);
  }

  unsigned char *ip;
  ip = (unsigned char *)&serveraddr.sin_addr.s_addr;
  printf("[INFO] Connected to %d.%d.%d.%d:%d.\n", ip[0], ip[1], ip[2], ip[3],
         port);

  return clientfd;
}

int main(int argc, char **argv) {
  // Check the args for the hostname and port of a Klondike service.
  if (argc != 3) {
    fprintf(stderr, "[WARN] usage: %s <host> <port>\n", argv[0]);
    exit(1);
  }

  // set up file descriptors
  int port = atoi(argv[2]); // TODO: check if it's really an int
  int client_fd = connect_to_server(argv[1], port);

  // set up client ID
  char *idstr = return_service_requested(client_fd, "i");
  if (idstr == NULL) {
    printf("[ERROR] No player ID given by the server.\n");
    return 1;
  }
  uint8_t player_id = (uint8_t)atoi(idstr); // TODO verify
  if (DEBUG) printf("[DEBUG] Got ID %d\n", player_id);

  // get the seed so we have the same deck
  char *seedstr = return_service_requested(client_fd, "s");
  if (seedstr == NULL) {
    printf("[ERROR] No seed given by the server.\n");
    return 1;
  }
  long seed = atol(seedstr); // TODO verify
  if (DEBUG)
    printf("[DEBUG] Got seed %li\n", seed);

  // initialize game
  solitaire_t *S = newSolitaire(seed);
  arena_t *arena = newArena(); // the user will have to manually update this

  char *feedback = malloc(80 * sizeof(char));
  while (true) {
    assert(S != NULL);

    // print game state
    if (arena->players > 0) putArena(arena);
    putSolitaire(S);

    // get input
    char cmd[80];
    if (!fgets(cmd, 80, stdin)) {
      putHelp("[ERROR] Error calling fgets.\n");
      continue;
    }

    // check for valid non-play commands
    if (cmd[0] == 'u') { // update the arena
      arena = get_arena(client_fd, player_id);
      continue; // will be warned of error if arena == NULL
    }

    // validate input locally before sending to server
    char *validation_error = validate_command(cmd, S);
    if (validation_error != 0) {
      putHelp(validation_error);
      continue;
    }

    // exit gracefully
    if (cmd[0] == 'q') {
      printf("Bye!\n");
      return 0;
    }

    // send input to the server
    if (DEBUG)
      printf("[DEBUG] Sending command to server: %s", cmd); // no \n necessary

    char *feedback = return_service_requested(client_fd, cmd);
    if (feedback == NULL) {
      printf("[ERROR] Server closed connection\n");
      exit(1);
    }

    if (DEBUG) printf("[DEBUG] Server replied with: %s\n", feedback);

    if (strcmp(feedback, "success") != 0) { // report errors
      printf("[ERROR] Error message from server:\n\t%s\n", feedback);

    } else { // update local state
      char *error_string = update(cmd, arena, S);

      // sanity check
      if (error_string != NULL) {
        printf("[ERROR] Server accepted invalid move. Updating arena and trying again.\n");
        arena = get_arena(client_fd, player_id);
        error_string = update(cmd, arena, S);
        if (error_string != NULL)
          printf("[ERROR] Server accepted truly invalid move.\n");
      }
    }
  }

  free(feedback);
  close(client_fd);
}
