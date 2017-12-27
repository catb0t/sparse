#include <criterion/criterion.h>
#include "../sparse.h"

Test(ns, count) {

  uint64_t b[34] = {1, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 34};

  cr_assert_eq(2, ns64_count_nonzero_elts(b, 34));

  uint64_t c[34] = {1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 34};

  cr_assert_eq(34, ns64_count_nonzero_elts(c, 34));

  cr_assert_eq(32, ns64_count_next_zeroes(b, 34, 1));
  cr_assert_eq(0, ns64_count_next_zeroes(c, 34, 1));
}

Test(ns, range) {
  uint64_t a[28] = {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1};
  uint64_t b[6] = {0, 7, 9, 15, 21, 26};

  uint64_t* res = ns64_zero_ranges(a, 28);

  cr_assert_arr_eq(b, res, ns64_count_nonzero_elts(a, 28));

  free(res);
  uint64_t c[] = {1,1,1,1,1,1,1,1,1};

  res = ns64_nonzero_elts(a, 28);

  cr_assert_arr_eq(c, res, ns64_count_nonzero_elts(a, 28));

  free(res);
}
