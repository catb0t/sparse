#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "sparse.h"

int main(void) {

  //uint64_t a[28] = {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1};
  sparse64_t b[11] = {5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5};
  size_t* const s = sparse64_uncompress_zero_ranges(b);

  for (size_t i = 0; i < sparse64_len_pairs(b) * 2; i += 2) {
    printf("%" PRIu64 " %" PRIu64 "\n", s[i], s[i + 1]);
  }

  return 0;
}
