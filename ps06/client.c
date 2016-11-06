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
#include "solitaire-lib.c"

#ifndef DEBUG
#define DEBUG 1
#endif

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
  printf("[INFO] Connected to server at %d.%d.%d.%d. Enter commands below:\n",
         ip[0], ip[1], ip[2], ip[3]);

  return clientfd;
}

int main(int argc, char **argv) {
  // Check the args for the hostname and port of a Klondike service.
  if (argc != 3) {
    fprintf(stderr, "[WARN] usage: %s <host> <port>\n", argv[0]);
    exit(0);
  }

  // get the seed so we have the same deck
  int client_fd = connect_to_server(argv[1], atoi(argv[2]));
  char *seedstr = read_some(client_fd);
  if (seedstr == NULL) {
    printf("[ERROR] No seed given by the server.\n");
    return 0;
  }
  long seed = atol(seedstr);

  // initialize game
  solitaire_t *S = newSolitaire(seed);
  arena_t *A = newArena();

  char *feedback = malloc(80 * sizeof(char));
  while (true) {
    // print game state
    putArena(A);
    putSolitaire(S);

    // get input
    char cmd[80];
    if (!fgets(cmd, 80, stdin)) {
      putHelp("[ERROR] Error calling fgets.\n");
      continue;
    }

    // validate input locally before sending to server
    char *validation_error = validate_command(cmd, S);
    if (validation_error != 0) {
      putHelp(validation_error);
    } else {

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

      // update local game state or report errors
      if (strcmp(feedback, "success") != 0) {
        printf("[ERROR] Error message from server:\n\t%s\n", feedback);
      } else {
        char *error_string = update(cmd, A, S);

        // sanity check
        if (error_string != NULL) {
          printf("[ERROR] Server accepted invalid move.\n");
        }
      }
    }
  }

  free(feedback);
  close(client_fd);
}
