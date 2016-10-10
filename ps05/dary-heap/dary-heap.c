/**
   @file
   @author Langston Barrett

   @section LICENSE

   See ../LICENSE.

   @section DESCRIPTION

   This file defines the dary-heap datastructure and operations on it.
*/

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 *  @brief A dary heap.
 *  @var d The maximum number of children per node.
 *  @var size The number of elements currently in the heap.
 *  @var max_size The size of the underlying array, the maximum number of
 *  elements in the heap.
 *  @var array The beginning of the array of elements.
 */
typedef struct {
  int64_t d;
  int64_t size;
  int64_t max_size;
  int64_t *array;
} dary_heap;

dary_heap *new_dary_heap(int64_t max_size, int64_t d) {
  dary_heap *heap = malloc(sizeof(dary_heap));
  heap->array = malloc(max_size*sizeof(int64_t));

  for (int64_t i = 0; i < max_size; i++) {
    heap->array = 0;
  }

  heap->d = d;
  return d;
}

/**
   @brief Get the child of a node.
   @param heap The binary heap in question.
   @param parent_index The index of the parent node.
   @param which An int in the range [0, d).
   @pre heap != NULL
   @return The index of that child.
*/
int64_t child_index(dary_heap *heap, int64_t parent_index, int64_t which) {
  assert(heap != NULL);
  return (heap->d * parent_index) + which; // di+1
}

int64_t parent_index(dary_heap *heap, int64_t child_index) {
  assert(heap != NULL);
  return floor((child_index - 1) / heap->d); // floor((i-1)/d)
}

int64_t *insert(dary_heap *heap, int64_t n) { return 0; }
