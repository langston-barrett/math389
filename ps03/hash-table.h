#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "llist.h"
#include "llist.gen.h"

typedef struct _dict {
  llist **buckets;
  int64_t size;
} dict;
