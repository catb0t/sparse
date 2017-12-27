#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "sparse.h"

int main(void) {

  uint64_t a[28] = {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1};

  for (size_t i = 0; i < 1000000; i++) {
    free(ns64_zero_ranges(a, 28));
  }


  return 0;
}
