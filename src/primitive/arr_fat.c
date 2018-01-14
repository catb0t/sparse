#include "../sparse.h"

/*
  uint64_t*, size_t -> uint64_t*

  copy the non-zero values from an array

  the return length is the same as ns64_count_nonzero_elts(array, len)
*/
uint64_t* ns64_nonzero_elts (const uint64_t* const array, const size_t len) {

  if (0 == len) { return alloc(uint64_t, len); }

  uint64_t* const out = alloc(uint64_t, ns64_count_nonzero_elts(array, len));

  // i = array; j = out
  for (size_t i = 0, j = 0; i < len; i++) {
    if ( 0 != array[i] ) {
      out[j++] = array[i];
    }
  }

  return out;
}

/*
  uint64_t*, size_t -> uint64_t*

  copy the non-zero values from an array, with their indexes in the original array

  the return length is the same as 2 * ns64_count_nonzero_elts(array, len)
*/
uint64_t* ns64_nonzero_elts_idxs (const uint64_t* const array, const size_t len) {

  if (0 == len) { return alloc(uint64_t, len); }

  const size_t out_len = 2 * ns64_count_nonzero_elts(array, len);

  uint64_t* const out = alloc(uint64_t, out_len);

  // i = array; j = out
  for (size_t i = 0, j = 0; i < len; i++) {
    if ( 0 != array[i] ) {
      out[j] = i;
      out[j + 1] = array[i];
      j += 2;
    }
  }

  return out;
}

/*
  return value has length 2 * count_real_zero_ranges(array, len)
*/
size_t* _real_zero_ranges (const uint64_t* const array, const size_t len) {
  const size_t out_len = 2 * _count_real_zero_ranges(array, len);
  size_t* const out = alloc(size_t, out_len);

  // i = array, j = out
  for (size_t i = 0, j = 0; i < len; i++) {
    if (0 == array[i]) {
      out[j] = i;
      out[j + 1] = i += ns64_count_next_zeroes(array, len, i) - 1;
      j += 2;
    }
  }

  return out;
}

/*
  return value has length 2 * ns64_count_adjc_nonzero_pairs(array, len)
*/
static size_t* adjc_nonzero_pairs (const uint64_t* const array, const size_t len) {

  const size_t out_len = 2 * ns64_count_adjc_nonzero_pairs(array, len);
  size_t* const out = alloc(size_t, out_len);

  // i = array, j = out
  for (size_t i = 0, j = 0; i < len; i++) {
    if (array[i] && i + 1 < len && array[i + 1]) {
      out[j] = array[i];
      out[j + 1] = array[i + 1];
      // skip
      j += 2;
    }
  }

  return out;
}

/*
  uint64_t*, size_t -> size_t*

  list the identifying upper and lower indexes of zero ranges in an array

  this function accounts for the implicit and necessary zero-length zero ranges
    between non-zero adjacent elements

  every individual data element exists above a zero range of length 0 or more
*/
size_t* ns64_zero_ranges (const uint64_t* const array, const size_t len) {
  // x 2 because a range is two numbers
  const size_t
    len_out = 2 * ns64_count_zero_ranges(array, len),
    len_realranges = 2 * _count_real_zero_ranges(array, len),
    len_adjc_pairs = ns64_count_adjc_nonzero_pairs(array, len);

  size_t
    * const realranges = _real_zero_ranges(array, len),
    * const adjc_pairs = adjc_nonzero_pairs(array, len),
    * const out = alloc(size_t, len_out);

  // i = array, j = out
  for (size_t i = 0; i < len; i++) {
    (void) len_realranges;
    (void) len_adjc_pairs;
  }

  free(realranges);
  free(adjc_pairs);

  return out;
}
