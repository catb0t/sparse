#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "rle.h"

int main(void) {

  rlep_t a[9] = {4, 0, 1, 1, 45, 121, 123, 899, 923};
  size_t b[8] = {0, 1, 2, 45, 124, 123, 1024, 923};
  //size_t c[4] = {0, 1, 121, 899};

  size_t* sp = _elements_data(a);

  for (size_t i = 0; i < 8; i += 2) {
    printf("%zu:%zu %zu:%zu\n", sp[i], sp[i + 1], b[i], b[i + 1]);
  }

  free(sp);
  return 0;
}
