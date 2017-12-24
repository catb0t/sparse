#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "rle.h"

int main(void) {

  #define BIG 100000000
  rlep_t* const b = alloc(rlep_t, BIG + 1);
  b[0] = BIG / 2;
  for (size_t i = 1; i < BIG + 1; i++) {
    b[i] = BIG;
  }

  rlep_search_idx_linear(b, BIG, NULL);

  free(b);
  return 0;
}
