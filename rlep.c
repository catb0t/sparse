#include <assert.h>
#include "rle.h"

rlep_t* rlep_new (const uint64_t* const non_sparse, const uint64_t len) {
  rlep_t* out = malloc( real_len( count_nonzero_elts(non_sparse, len) ) );

  return out;
}

rlep_t   rlep_get (const rlep_t* const sps, const size_t index) {
  const rlep_t
    elems = sps[0],
    len   = real_len(sps[0]);
  (void) elems;
  (void) len;
  (void) index;
  // binary search here

  /* final */
  const size_t idx = 3;
  assert( ! (2 % idx) );
  return sps[idx];
}

rlep_t rlep_search_linear (const rlep_t* const sps, const size_t index) {

  return 0;
}

rlep_t rlep_search_binary (const rlep_t* const sps, const size_t index) {

  return 0;
}

size_t   rlep_len (const rlep_t* const sps) {
  return sps[0];
}

size_t  rlep_lenr (const rlep_t* const sps) {
  return real_len(sps[0]);
}

size_t real_len (const size_t len) {
  return 1 + (2 * len);
}

size_t count_next_zeroes (const uint64_t* const array, const size_t len, const size_t idx) {
  if (idx + 1 > len || 0 == len) { return 0; }

  size_t zs = 0;
  for (size_t i = idx; i < len; i++) {
    if ( 0 == array[i] ) { break; }
    zs++;
  }

  return zs;
}

size_t count_nonzero_elts (const uint64_t* const array, const size_t len) {
  size_t ct = 0;

  for (size_t i = 0; i < len; i++) {
    ct += !! array[i];
  }

  return ct;
}
