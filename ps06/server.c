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

/*************************** Definitions ***************************/

#define MAXLINE 80
#define CONNECTIONS 128

typedef struct _client_t {
  int connection;
  uint8_t id;
  long seed; // used to generate deck
  arena_t *arena; // which game they're in
  struct sockaddr_in address;
} client_t;

/*************************** Playing ***************************/
char *respond(client_t *client, char *cmd, solitaire_t *solitaire) {
  assert(cmd != NULL);
  assert(solitaire != NULL);
  assert(client != NULL);
  assert(client->arena != NULL);

  // check for valid non-play commands
  if (cmd[0] == 'u') {
    return arena_to_str(client->arena);

  } else if (cmd[0] == 's') { // return client's seed
    char *seedstr = malloc(11*sizeof(char)); // log_10(LONG_MAX = 2147483647)+1
    sprintf(seedstr, "%lu", client->seed);
    return seedstr;

  } else if (cmd[0] == 'i') { // return client's id
    char *idstr = malloc(4*sizeof(char)); // log_10(UINT8_MAX = 127)+1
    sprintf(idstr, "%d", client->id);
    return idstr;

  } else {
    // validate command, attempt to update game state
    char *error = validate_command(cmd, solitaire);
    if (error != NULL) {
      printf("[DEBUG] Validation error, client #%d: %s\n", client->id, error);
      return error;
    } else {
      error = update(cmd, client->arena, solitaire);
      if (error != NULL) {
        printf("[DEBUG] Update error, client #%d: %s\n", client->id, error);
        return error;
      } else {
        printf("[DEBUG] Success, client #%d\n", client->id);
        if (DEBUG)
          putArena(client->arena);
        if (DEBUG)
          putSolitaire(solitaire);
        return "success";
      }
    }
  }
  printf("[ERROR] Should be unreachable!\n");
  return NULL;
}

// play a Klondike game with a single client
void *play_with_client(void *ci) {
  assert(ci != NULL);

  /********************* Setup *********************/
  char *cmd = malloc(MAXLINE * sizeof(char)); // receive from client
  client_t *client = (client_t *)ci;
  assert(client != NULL);
  assert(client->arena != NULL);
  assert(client->arena->lock != NULL);

  // add a player to the arena, add their stacks to the arena
  pthread_mutex_lock(client->arena->lock);
  client->arena->players++;
  pthread_mutex_unlock(client->arena->lock);

  solitaire_t *solitaire = newSolitaire(client->seed);

  /********************* Networking *********************/
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

  // get the initial command
  cmd = read_some(client->connection);

  /********************* Gameplay *********************/
  while (cmd != NULL) { // while the game is still going...
    char *response = respond(client, cmd, solitaire);
    printf("[INFO] Replying to #%d: %s\n", client->id, response);
    cmd = return_service_requested(client->connection, response);
    printf("[INFO] Received message from #%d: %s", client->id, cmd);
  }
  printf("[INFO] Client #%d disconnected.\n", client->id);

  // remove from the arena
  pthread_mutex_lock(client->arena->lock);
  client->arena->players--;
  pthread_mutex_unlock(client->arena->lock);

  close(client->connection);
  return NULL;
}

int main(int argc, char **argv) {
  // Make sure we've been given a port to listen on.
  if (argc != 2) {
    fprintf(stderr, "[WARN] usage: %s <port>\n", argv[0]);
    return 1;
  }

  // initialize game
  arena_t *arena = newArena();

  // Open a socket to listen for client connections.
  int listenfd;
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "[ERROR] Socket creation failed.\n");
    return 1;
  }

  // Build the service's info into a (struct sockaddr_in).
  int port = atoi(argv[1]); // TODO verify
  if (port == 0) {
    fprintf(stderr, "[ERROR] Invalid port '%s'\n", argv[1]);
    return 1;
  }
  struct sockaddr_in serveraddr;
  bzero((char *)&serveraddr, sizeof(struct sockaddr_in));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)port);

  // Bind that socket to a port.
  if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr)) <
      0) {
    fprintf(stderr, "[ERROR] Bind failed.\n");
    return 1;
  }

  // Listen for client connections on that socket.
  if (listen(listenfd, CONNECTIONS) < 0) {
    fprintf(stderr, "[ERROR] Listen failed.\n");
    return 1;
  }

  fprintf(stderr, "[INFO] Klondike server listening on port %d...\n", port);

  uint8_t clients = 0;

  // Handle client sessions.
  while (1) {
    client_t *client = malloc(sizeof(client_t));
    client->id = clients + 1; // client_id != 0, and can be parsed & verified
    client->arena = arena;
    clients++;


    // set up seed to generate deck
    struct timeval tp;
    gettimeofday(&tp, NULL);
    client->seed = tp.tv_sec; // seed != 0, and can be parsed & verified

    // Accept a connection from a client, get a file descriptor for
    // communicating with the client.
    socklen_t acceptlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    if ((client->connection = accept(listenfd, (struct sockaddr *)&clientaddr,
                                     &acceptlen)) < 0) {
      fprintf(stderr, "[WARN] Accept failed.\n");
      return 1;
    }

    // create a thread to handle the client
    pthread_t tid;
    pthread_create(&tid, NULL, play_with_client, (void *)client);
  }

  close(listenfd);
  return 0;
}
