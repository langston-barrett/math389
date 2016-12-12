#ifndef _tsand_h
#define _tsand_h
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include "./sand-lib.gen.h"

pile_ *tevolve(pile_ *src);

typedef struct _barrier_ {
  uint8_t total;
  uint8_t here;
  uint8_t done;
  pthread_mutex_t *lock;
  pthread_cond_t *cond;
} barrier_;

typedef struct thread_args {
  pile_ *src;
  pile_ *dst;
  pile_ **fin; // pointer to the "return pile"
  uint64_t max;
  barrier_ *barrier;
  uint8_t id;
} thread_args;

#endif
