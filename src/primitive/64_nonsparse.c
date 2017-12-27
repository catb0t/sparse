#include "../sparse.h"

size_t ns64_count_next_zeroes (const uint64_t* const array, const size_t len, const size_t idx) {
  if (idx + 1 > len || 0 == len) { return 0; }

  size_t zs = 0;
  for (size_t i = idx; i < len; i++) {
    if ( 0 != array[i] ) { break; }
    zs++;
  }

  return zs;
}

size_t ns64_count_nonzero_elts (const uint64_t* const array, const size_t len) {
  size_t ct = 0;

  for (size_t i = 0; i < len; i++) {
    ct += !! array[i];
  }

  return ct;
}

size_t ns64_count_adjacent_nonzero_elts (const uint64_t* const array, const size_t len) {
  (void) array;
  (void) len;
  return 0;
}

uint64_t* ns64_nonzero_elts (const uint64_t* const array, const size_t len) {

  if (0 == len) {
    return alloc(uint64_t, len);
  }

  uint64_t* const out = alloc(uint64_t, ns64_count_nonzero_elts(array, len));

  size_t j = 0;

  // i = array; j = out
  for (size_t i = 0; i < len; i++) {
    if ( 0 != array[i] ) {
      out[j++] = array[i];
    }
  }

  return out;

}

size_t* ns64_zero_ranges (const uint64_t* const array, const size_t len) {
  (void) array;
  (void) len;
  return NULL;
}

static size_t real_zero_ranges (const uint64_t* const array, const size_t len) {
  size_t ranges = 0;

  for (size_t i = 0; i < len; i++) {
    if (0 == array[i]) {
      ranges++;
      i += ns64_count_next_zeroes(array, len, i) - 1;
    }
  }

  return ranges;
}

size_t ns64_count_zero_ranges (const uint64_t* const array, const size_t len) {
  // there is a zero range between two nonzero elements, it has length zero

  return real_zero_ranges(array, len) + ns64_count_adjacent_nonzero_elts(array, len) - 1;
}
