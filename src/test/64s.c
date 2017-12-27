#include <stdio.h>
#include <criterion/criterion.h>
#include "../sparse.h"

Test(prim, len) {
  sparse64_t a[9] = {4, 0, 1, 0, 2, 121, 123, 899, 923};
  cr_assert_eq(4, sparse64_len(a));
  cr_assert_eq(9, sparse64_lenr(a));

  cr_assert_eq(4 + 1020, sparse64_len_virtual(a));
}

Test(prim, lsch) {

  sparse64_t a[9] = {4, 0, 1, 1, 45, 121, 123, 899, 923};
               /*l  a  d  a  d  a    d     a    d*/

  cr_assert_eq(1, sparse64_search_idx_linear(a, 0, NULL));
  cr_assert_eq(0, sparse64_search_idx_linear(a, 1, NULL));
  cr_assert_eq(45, sparse64_search_idx_linear(a, 2, NULL));
  cr_assert_eq(0, sparse64_search_idx_linear(a, 3, NULL));

  #define BIG 10000000
  sparse64_t* const b = alloc(sparse64_t, BIG + 1);
  b[0] = BIG / 2;
  for (size_t i = 1; i < BIG + 1; i++) {
    b[i] = BIG;
  }

  cr_assert_eq(BIG, sparse64_search_idx_linear(b, BIG, NULL));

  free(b);
}

Test(prim, elts) {
  sparse64_t a[9] = {4, 0, 1, 1, 45, 121, 123, 899, 923};
  size_t b[8] = {0, 1, 2, 45, 124, 123, 1024, 923};
  size_t c[4] = {0, 1, 121, 899};

  size_t* sp = _elements_addr_64(a);

  cr_assert_arr_eq(c, sp, 4);

  free(sp);

  sp = _elements_data_64(a);

  cr_assert_arr_eq(b, sp, 8);

  free(sp);
}

Test(prim, uncompress) {
  /*                   0      2      124           1024*/
  sparse64_t a[9] = {4, 0, 1, 1, 45, 121, 98,   899,    923};
  size_t c[8] = {  0,0,  1,2,   3,124,    125,1024     };

  size_t* f = sparse64_uncompress_zero_ranges(a);

  cr_assert_arr_eq(f, c, 8);

  free(f);
  /*                   4       12       24        39*/
  sparse64_t b[9] = {4, 4, 2,  7,   3,  11,  4,  14,   5};
  size_t d[8] = {  0,4,   5,12,    13,24,   25,39    };

  f = sparse64_uncompress_zero_ranges(b);

  cr_assert_arr_eq(f, d, 8);

  free(f);
}

Test(prim, in_range) {
  cr_assert(! is_in_range_64(0, 0, 0));
  cr_assert(  is_in_range_64(1, 1, 2));
}

