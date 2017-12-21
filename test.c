#include <criterion/criterion.h>
#include <stdio.h>
#include "rle.h"

Test(prim, len) {
  rlep_t a[] = {4, 0, 1, 0, 2, 121, 123, 899, 923};
  cr_assert_eq(4, rlep_len(a));
  cr_assert_eq(9, rlep_lenr(a));
}

Test(prim, count) {

  uint64_t b[34] = {1, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 34};

  cr_assert_eq(2, count_nonzero_elts(b, 34));

  uint64_t c[34] = {1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 34};

  cr_assert_eq(34, count_nonzero_elts(c, 34));

  cr_assert_eq(33, count_next_zeroes(b, 34, 1));
  cr_assert_eq(0, count_next_zeroes(c, 34, 1));
}

Test(prim, get) {

}
