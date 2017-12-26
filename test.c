#include <criterion/criterion.h>
#include <stdio.h>
#include "rle.h"

Test(prim, len) {
  rlep_t a[9] = {4, 0, 1, 0, 2, 121, 123, 899, 923};
  cr_assert_eq(4, rlep_len(a));
  cr_assert_eq(9, rlep_lenr(a));

  cr_assert_eq(4 + 1020, rlep_len_virtual(a));
}

Test(prim, ns_count) {

  uint64_t b[34] = {1, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 34};

  cr_assert_eq(2, ns_count_nonzero_elts(b, 34));

  uint64_t c[34] = {1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 34};

  cr_assert_eq(34, ns_count_nonzero_elts(c, 34));

  cr_assert_eq(32, ns_count_next_zeroes(b, 34, 1));
  cr_assert_eq(0, ns_count_next_zeroes(c, 34, 1));
}

Test(prim, lsch) {

  rlep_t a[9] = {4, 0, 1, 1, 45, 121, 123, 899, 923};
               /*l  a  d  a  d  a    d     a    d*/

  cr_assert_eq(1, rlep_search_idx_linear(a, 0, NULL));
  cr_assert_eq(0, rlep_search_idx_linear(a, 1, NULL));
  cr_assert_eq(45, rlep_search_idx_linear(a, 2, NULL));
  cr_assert_eq(0, rlep_search_idx_linear(a, 3, NULL));

  #define BIG 10000000
  rlep_t* const b = alloc(rlep_t, BIG + 1);
  b[0] = BIG / 2;
  for (size_t i = 1; i < BIG + 1; i++) {
    b[i] = BIG;
  }

  cr_assert_eq(BIG, rlep_search_idx_linear(b, BIG, NULL));

  free(b);
}

Test(prim, elts) {
  rlep_t a[9] = {4, 0, 1, 1, 45, 121, 123, 899, 923};
  size_t b[8] = {0, 1, 2, 45, 124, 123, 1024, 923};
  size_t c[4] = {0, 1, 121, 899};

  size_t* sp = _pelements_addr(a);

  cr_assert_arr_eq(c, sp, 4);

  free(sp);

  sp = _pelements_data(a);

  cr_assert_arr_eq(b, sp, 8);

  free(sp);
}

Test(prim, dcz) {
  /*                   0      2      124           1024*/
  rlep_t a[9] = {4, 0, 1, 1, 45, 121, 98,   899,    923};
  size_t c[8] = {  0,0,  1,2,   3,124,    125,1024     };

  size_t* f = rlep_decode_zero_ranges(a);

  cr_assert_arr_eq(f, c, 8);

  free(f);
  /*                   4       12       24        39*/
  rlep_t b[9] = {4, 4, 2,  7,   3,  11,  4,  14,   5};
  size_t d[8] = {  0,4,   5,12,    13,24,   25,39    };

  f = rlep_decode_zero_ranges(b);

  cr_assert_arr_eq(f, d, 8);

  free(f);
}

Test(prim, in_range) {
  cr_assert(! is_in_range(0, 0, 0));
  cr_assert(  is_in_range(1, 1, 2));
}

Test(prim, ns_range) {
  uint64_t a[28] = {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1};
  uint64_t b[6] = {0, 7, 9, 15, 21, 26};

  uint64_t* res = ns_zero_ranges(a, 28);

  cr_assert_arr_eq(b, res, 6);

  free(res);
  uint64_t c[] = {1,1,1,1,1,1,1,1,1};

  res = ns_nonzero_elts(a, 28);

  cr_assert_arr_eq(c, res, 9);

  free(res);
}
