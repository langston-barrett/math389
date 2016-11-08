#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include "./solitaire-lib.c"

#ifndef DEBUG
#define DEBUG 1
#endif

#define MAXLINE 80
#define CONNECTIONS 128

typedef struct _client_t {
  int id;
  int connection;
  struct sockaddr_in address;
} client_t;

// play a Klondike game with a single client
void *play_with_client(void *ci) {
  /********************* Networking *********************/

  assert(ci != NULL);
  client_t *client = (client_t *)ci;
  assert(client != NULL);

  // Report the client that connected.
  struct hostent *hostp;
  if ((hostp = gethostbyaddr((const char *)&client->address.sin_addr.s_addr,
                             sizeof(struct in_addr), AF_INET)) == NULL) {
    fprintf(stderr, "[WARN] gethostbyaddr failed.");
  } else {
    // Record that the client connected.
    printf("[INFO] Accepted connection from client %d %s (%s)\n", client->id,
           hostp->h_name, inet_ntoa(client->address.sin_addr));
  }

  /********************* Setup *********************/

  char *cmd = malloc(MAXLINE * sizeof(char)); // receive from client
  char *msg = malloc(MAXLINE * sizeof(char)); // send to client

  // send client seed to generate deck with
  struct timeval tp;
  gettimeofday(&tp, NULL);
  char seedstr[80];
  long seed = tp.tv_sec;
  sprintf(seedstr, "%li", seed);
  cmd = return_service_requested(client->connection, seedstr);

  // initialize game
  solitaire_t *S = newSolitaire(seed);
  arena_t *A = newArena();

  /********************* Gameplay *********************/
  // while the game is still going...
  while (cmd != NULL) {
    // validate command, attempt to update game state
    char *error = validate_command(cmd, S);
    if (error != NULL) {
      printf("[DEBUG] Validation error, client #%d: %s\n", client->id, error);
      msg = error;
    } else {
      error = update(cmd, A, S);
      if (error != NULL) {
        printf("[DEBUG] Update error, client #%d: %s\n", client->id, error);
        msg = error;
      } else {
        printf("[DEBUG] Success, client #%d\n", client->id);
        if (DEBUG)
          putArena(A);
        if (DEBUG)
          putSolitaire(S);
        msg = "success";
      }
    }

    printf("[INFO] Replying to #%d: %s\n", client->id, msg);
    cmd = return_service_requested(client->connection, msg);
    printf("[INFO] Received message from #%d: %s", client->id, cmd);
  }
  printf("[INFO] Client #%d disconnected.\n", client->id);
  close(client->connection);
  free(client);
  free(ci);
}

int main(int argc, char **argv) {
  // Make sure we've been given a port to listen on.
  if (argc != 2) {
    fprintf(stderr, "[WARN] usage: %s <port>\n", argv[0]);
    exit(1);
  }

  // Open a socket to listen for client connections.
  int listenfd;
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "[ERROR] Socket creation failed.\n");
    exit(1);
  }

  // Build the service's info into a (struct sockaddr_in).
  int port = atoi(argv[1]);
  struct sockaddr_in serveraddr;
  bzero((char *)&serveraddr, sizeof(struct sockaddr_in));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)port);

  // Bind that socket to a port.
  if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr)) <
      0) {
    fprintf(stderr, "[ERROR] Bind failed.\n");
    exit(1);
  }

  // Listen for client connections on that socket.
  if (listen(listenfd, CONNECTIONS) < 0) {
    fprintf(stderr, "[ERROR] Listen failed.\n");
    exit(1);
  }

  fprintf(stderr, "[INFO] Klondike server listening on port %d...\n", port);

  uint8_t clients = 0;

  // Handle client sessions.
  while (1) {
    /* TODO minimize this code! dispatch sooner */

    client_t *client = malloc(sizeof(client_t));
    client->id = clients;
    clients++;

    // Accept a connection from a client, get a file descriptor for
    // communicating with the client.
    socklen_t acceptlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    if ((client->connection = accept(listenfd, (struct sockaddr *)&clientaddr,
                                     &acceptlen)) < 0) {
      fprintf(stderr, "[WARN] Accept failed.\n");
      exit(-1);
    }

    // create a thread to handle the client
    pthread_t tid;
    pthread_create(&tid, NULL, play_with_client, (void *)client);
  }

  close(listenfd);
  return 0;
}
