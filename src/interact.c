#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "sparse.h"

int main(void) {

  uint64_t a[9] = {0,0,0,1,1,1,0,1,1};

  printf("%zu\n", ns64_count_zero_ranges(a, 9));

  return 0;
}
