#include "../sparse.h"

/*
  uint64_t*, size_t, size_t -> size_t

  number of contiguous zeroes after an index, including the index

  does not account for special case of zero at end of array
*/
size_t ns64_count_next_zeroes (const uint64_t* const array, const size_t len, const size_t idx) {
  // invalid values
  if (idx >= len || 0 == len) { return 0; }
  // also invalid, maybe workable though
  if (idx + 1 >= len) { return !! array[idx]; }

  size_t zs = 0;
  for (size_t i = idx; i < len; i++) {
    if ( 0 != array[i] ) { break; }
    zs++;
  }

  return zs;
}

/*
  uint64_t*, size_t -> size_t

  number of non-zero (data) elements in an array

  does not include a zero element at the end of an array as a special case

  instead literally counts just non-zero elements
*/
size_t ns64_count_nonzero_elts (const uint64_t* const array, const size_t len) {
  size_t ct = 0;

  for (size_t i = 0; i < len; i++) {
    ct += !! array[i];
  }

  return ct;
}

/*
  uint64_t*, size_t -> size_t

  count the number of overlapping pairs of adjacent nonzero elements

  for example, the snippet <0, 1, 0> has 0 overlapping pairs of nonzero elements

  the snippet <1, 1, 1> has 2 overlapping pairs which share the middle element,
    this number is one fewer than the size of the contiguous non-zero range
*/
size_t ns64_count_adjc_nonzero_pairs (const uint64_t* const array, const size_t len) {

  size_t elts = 0;

  for (size_t i = 0; i < len; i++) {
    if (array[i] && i + 1 < len && array[i + 1]) {
      // incrementing i here would not count overlapping pairs
      ++elts;
    }
  }

  return elts;
}

/*
  uint64_t*, size_t -> size_t

  count the number of "real" ranges of zeroes in the array

  this function does not count implicit zero ranges between adjacent nonzero
    elements; use ns64_count_adjc_nonzero_pairs for that
*/
size_t _count_real_zero_ranges (const uint64_t* const array, const size_t len) {
  size_t ranges = 0;

  for (size_t i = 0; i < len; i++) {
    if (0 == array[i]) {
      ranges++;
      // skip the "body" of the range
      i += ns64_count_next_zeroes(array, len, i) - 1;
    }
  }

  return ranges;
}

/*
  uint64_t*, size_t -> size_t

  count all the zero ranges in an array

  this function is composed of (static) count_real_zero_ranges and ns64_count_zero_ranges

  therefore it returns the number of zero-length zero ranges which exist between
    adjacent nonzero elements too
*/
size_t ns64_count_zero_ranges (const uint64_t* const array, const size_t len) {
  // there is a zero range between two nonzero elements, it has length zero
  return _count_real_zero_ranges(array, len) + ns64_count_adjc_nonzero_pairs(array, len);
}
