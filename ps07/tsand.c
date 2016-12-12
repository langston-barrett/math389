#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./tsand.h"
#include "./sand-lib.h"

#define THREADS 4
#define DEBUG 0

static barrier_ *new_barrier(uint8_t threads) {
  barrier_ *barrier = malloc(sizeof(barrier_));
  assert(barrier != NULL);
  barrier->total = threads;
  barrier->here = 0;
  barrier->done = 0;

  // lock
  barrier->lock = malloc(sizeof(pthread_mutex_t));
  assert(barrier->lock != NULL);
  assert(pthread_mutex_init(barrier->lock, NULL) == 0);
  assert(barrier->lock != NULL);

  // condition variable
  barrier->cond = malloc(sizeof(pthread_cond_t));
  assert(barrier->cond != NULL);
  assert(pthread_cond_init(barrier->cond, NULL) == 0);
  assert(barrier->cond != NULL);

  return barrier;
}

static void __attribute__((nonnull))
barrier_wait(uint8_t id, barrier_ *barrier, bool done, pile_ *dst,
             pile_ **fin) {
  assert(barrier != NULL);
  assert(dst != NULL);
  assert(fin != NULL);

  if (DEBUG)
    printf("thread %d: in barrier_wait\n", id);
  pthread_mutex_lock(barrier->lock);
  if (DEBUG)
    printf("thread %d: locked the mutex\n", id);

  // I'm here!
  barrier->here++;
  if (done)
    barrier->done++;

  // are we all done waiting?
  if (barrier->here >= barrier->total) {
    /* if (DEBUG) */
    /*   print_pile(dst, true); */

    // reset the done counter each time
    if (barrier->done < barrier->total)
      barrier->done = 0;
    else {
      if (DEBUG)
        printf("thread %d: all done!\n", id);
      *fin = dst;
    }

    if (DEBUG)
      printf("thread %d: all here!\n", id);

    barrier->here = 0;
    pthread_cond_broadcast(barrier->cond);
  } else { // this will unlock while it waits
    if (barrier->cond != NULL && barrier->lock != NULL)
      pthread_cond_wait(barrier->cond, barrier->lock);
  }

  if (DEBUG)
    printf("thread %d: unlocked the mutex\n", id);

  pthread_mutex_unlock(barrier->lock);
}

static void __attribute__((nonnull)) *work(void *thread_arg) {
  thread_args *arg = (thread_args *)thread_arg;
  assert(arg->src != NULL);
  assert(arg->src->grid != NULL);
  assert(arg->dst != NULL);
  assert(arg->dst->grid != NULL);

  // divvy up the grid into four quarters
  uint16_t row_beg = 0;
  uint16_t row_end = 0;
  uint16_t col_beg = 0;
  uint16_t col_end = 0;
  // TODO: refactor for a 1D array
  switch (arg->id) {
  case 1: // quadrant I
    // row_beg = 0;
    row_end = (arg->src->rows / 2) + 1;
    col_beg = (arg->src->cols / 2) + 1;
    col_end = arg->src->cols;
    break;
  case 2: // quadrant II
    // row_beg = 0;
    // col_beg = 0;
    row_end = (arg->src->rows / 2) + 1;
    col_end = (arg->src->cols / 2) + 1;
    break;
  case 3: // quadrant III
    // col_beg = 0;
    row_beg = (arg->src->rows / 2) + 1;
    row_end = arg->src->rows;
    col_end = (arg->src->cols / 2) + 1;
    break;
  default: // quadrant IV
    row_beg = (arg->src->rows / 2) + 1;
    row_end = arg->src->rows;
    col_beg = (arg->src->cols / 2) + 1;
    col_end = arg->src->cols;
    break;
  }

  if (DEBUG) {
    printf("thread %d: row_beg: %d\n", arg->id, row_beg);
    printf("thread %d: row_end: %d\n", arg->id, row_end);
    printf("thread %d: col_beg: %d\n", arg->id, col_beg);
    printf("thread %d: col_end: %d\n", arg->id, col_end);
    for (uint16_t i = 0; i < arg->src->rows; i++) {
      for (uint16_t j = 0; j < arg->src->cols; j++) {
        if (i >= row_beg && i < row_end && j >= col_beg && j < col_end)
          printf("X");
        else
          printf("O");
      }
      printf(" (%d)\n", arg->id);
    }
  }

  while (arg->barrier->done < arg->barrier->total) {
    bool done =
        step_region(arg->src, arg->dst, row_beg, row_end, col_beg, col_end);

    barrier_wait(arg->id, arg->barrier, done, arg->dst, arg->fin);

    pile_ *tmp = arg->src; // switch src and dst
    arg->src = arg->dst;
    arg->dst = tmp;
  }

  free(arg);

  if (DEBUG)
    printf("thread %d: done, exiting\n", arg->id);
  pthread_exit(NULL);
}

// take a pile forward until it settles. printing optional. color optional.
pile_ *tevolve(pile_ *src) {
  assert(src != NULL);
  assert(src->grid != NULL);
  assert(src->max % 4 == 0);
  assert(src->rows > THREADS);
  assert(src->cols > THREADS);

  // we'll synchronously switch between data in this pile and the other
  pile_ *dst = new_pile(src->rows, src->cols, 1, src->max);
  assert(dst != NULL); // make sure we have the memory

  // this pile will hold the return value of the threads
  pile_ **fin = malloc(sizeof(pile_ *));
  assert(fin != NULL); // make sure we have the memory

  // init a barrier
  barrier_ *barrier = new_barrier(THREADS);

  // spin up threads
  pthread_t threads[THREADS];
  for (uint8_t i = 0; i < THREADS; i++) {
    thread_args *args = malloc(sizeof(thread_args));
    assert(args != NULL);
    args->id = i;
    args->src = src;
    args->dst = dst;
    args->fin = fin;
    args->barrier = barrier;
    int rc = pthread_create(&threads[i], NULL, work, (void *)args);
    assert(rc == 0);
  }

  // wait until complete before returning
  for (uint8_t i = 0; i < THREADS; i++) {
    if (DEBUG)
      printf("main thread: waiting on %d\n", i);
    assert(pthread_join(threads[i], NULL) == 0);
  }

  if (DEBUG)
    fputs("main thread: returning\n", stdout);

  free(barrier);

  assert(src != NULL);
  assert(src->grid != NULL);
  assert(dst != NULL);
  assert(dst->grid != NULL);
  assert(fin != NULL);
  assert(*fin != NULL);
  return *fin;
}
