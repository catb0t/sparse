#include "rle.h"

size_t ns_count_next_zeroes (const uint64_t* const array, const size_t len, const size_t idx) {
  if (idx + 1 > len || 0 == len) { return 0; }

  size_t zs = 0;
  for (size_t i = idx; i < len; i++) {
    if ( 0 != array[i] ) { break; }
    zs++;
  }

  return zs;
}

size_t ns_count_nonzero_elts (const uint64_t* const array, const size_t len) {
  size_t ct = 0;

  for (size_t i = 0; i < len; i++) {
    ct += !! array[i];
  }

  return ct;
}

uint64_t* ns_nonzero_elts (const uint64_t* const array, const size_t len) {

  if (0 == len) { return alloc(uint64_t, 0); }

  uint64_t* const out = alloc(uint64_t, len);

  size_t j = 0;

  // i = array; j = out
  for (size_t i = 0; i < len; i++) {
    if (0 != array[i]) {
      out[j++] = array[i];
    }
  }

  return realloc(out, (1 + j) * sizeof (uint64_t));

}

uint64_t* ns_zero_ranges (const uint64_t* const array, const size_t len) {

  if (0 == len) { return alloc(uint64_t, 1); }

  uint64_t* const out = alloc(uint64_t, len * 2);

  size_t j = 0;

  // i = len; j = out
  for (size_t i = 0; i < len; i++, j += 2) {
    if (0 == array[i]) {
      out[j] = i;
      out[j + 1] = i + ns_count_next_zeroes(array, len, i);
    }
  }

  return out; //realloc(out, (1 + j) * sizeof (uint64_t));

}
