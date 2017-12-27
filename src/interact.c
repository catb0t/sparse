#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "sparse.h"

int main(void) {

  uint64_t a[28] = {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1};
  //uint64_t b[6] = {0, 7, 9, 15, 21, 26};

/*  uint64_t* res = ns64_zero_ranges(a, 28);

  size_t len = ns64_count_zero_ranges(a, 28);

  printf("%zu\n", len);

  for (size_t i = 0; i < len; i++) {
    printf("%" PRIu64 "\n", res[i]);
  }

  free(res);
*/  //uint64_t c[] = {1,1,1,1,1,1,1,1,1};

  uint64_t* res = ns64_nonzero_elts(a, 28);

  size_t len = ns64_count_nonzero_elts(a, 28);

  printf("%zu\n", len);

  for (size_t i = 0; i < len; i++) {
    printf("%" PRIu64 " %zu\n", res[i], i);
  }

  free(res);

  return 0;
}
