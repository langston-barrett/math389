#ifndef __tsand_h
#define __tsand_h
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include "./sand-lib.gen.h"

pile *tevolve(pile *src);

typedef struct barrier {
  uint8_t total;
  uint8_t here;
  uint8_t done;
  pthread_mutex_t *lock;
  pthread_cond_t *cond;
} barrier;

typedef struct thread_args {
  pile *src;
  pile *dst;
  pile **fin; // pointer to the "return pile"
  uint64_t max;
  barrier *barrier;
  uint8_t id;
} thread_args;

#endif
