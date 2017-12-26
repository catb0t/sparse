#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "sparse.h"

int main(void) {

  uint64_t a[28] = {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1};
  //uint64_t b[6] = {0, 7, 9, 15, 21, 26};

  size_t len;

  uint64_t* res = ns_zero_ranges(a, 28, &len);

  free(res);
  //uint64_t c[] = {1,1,1,1,1,1,1,1,1};

  res = ns_nonzero_elts(a, 28, &len);

  free(res);
  return 0;
}
