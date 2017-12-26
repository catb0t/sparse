#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <inttypes.h>
#include "rle.h"

rlep_t* rlep_new (const uint64_t* const non_sparse, const uint64_t len) {
  rlep_t* const out = malloc( _real_len( ns_count_nonzero_elts(non_sparse, len) ) );

  return out;
}

/*
  rlep_t*, size_t -> rlep_t

  get a value from a primitive rle array by its index.

  normally, this is done by adding the index value to the pointer value, to get
    the offset of the element you want.

  with a sparse array, because it compresses sequences of zeroes, then indexing
    is no longer O(1), but instead necessitates a search of some sort.

*/
rlep_t   rlep_get (const rlep_t* const sps, const size_t index, bool* const ok) {

  return rlep_search_idx_linear(sps, index, ok);

}

/*
  it is important to note that the rlep_search_idx_* functions search for an *index*!
  not for a value.
*/


rlep_t rlep_search_idx_linear (const rlep_t* const sps, const size_t get_index, bool* const ok) {

  if ( NULL != ok ) {
    *ok = true;
  }

  const size_t vlen = rlep_len_virtual(sps);

  if (get_index > vlen) {
    errno = EINVAL;
    if (NULL != ok) { *ok = false; }
    return 0;
  }

  const size_t len = rlep_len(sps), rlen = 2 * len;

  size_t* const zero_ranges = rlep_decode_zero_ranges(sps);

  // if the index is within a zero range the value is zero
  for (size_t i = 0; i < rlen; i += 2) {
    if ( is_in_range(get_index, zero_ranges[i], zero_ranges[i + 1]) ) {
      return 0;
    }
  }

  free(zero_ranges);

  size_t* const elts = _pelements_data(sps);

  for (size_t i = 0; i < rlen; i += 2) {
    if (get_index == elts[i]) {
      const size_t val = elts[i + 1];
      free(elts);
      return val;
    }
  }

  free(elts);

  // no entry
  errno = ENOENT;

  if ( NULL != ok ) {
    *ok = false;
  }
  return 0;
}

rlep_t rlep_search_idx_binary (const rlep_t* const sps, const size_t index, bool* const ok) {
  (void) sps;
  (void) index;
  (void) *ok;
  return 0;
}

/*
  uint64_t* -> size_t*

  runs of one or more zeroes to ranges
*/
size_t* rlep_encode_zero_ranges (const uint64_t* const sps) {
  const size_t len = rlep_len(sps), rlen = 2 * len;

  size_t* out = alloc(size_t, rlen);



  return out;
}

/*
  rlep_t* -> size_t*

  list of ranges of zero indexes in the given array

  convert the relative 0-sizes from their real representation in a sparse array
    (which is given by sparse_addrs) to ranges

  ranges include their top index value, but not their lower index value.

  these ranges represent a number of 0 elements between two indexes by the
    differential between their elements

  the range, (0, 0), has a difference of 0 between its elements. it represents a
    0-length span of zeroes, at the 0th index of an array

  the range, (0, 1), has a difference of 1 between its elements. it represents a
    1-length span of 0s at the 0th index; there is a nonzero element in the 1st index

  the range, (1, 2), has a differential of 1. it represents a 1-length span of
    0s between indexes 1 and 2. the index 2 has a nonzero element
*/

size_t* rlep_decode_zero_ranges (const rlep_t* const sps) {
  const size_t len = rlep_len(sps), rlen = 2 * len;

  size_t
    * const addrs = _pelements_addr(sps),   // has length 'len'
    * const out   = alloc(size_t, rlen), // has length 'rlen'
      index_total = 0;

  // i = addrs; j = out
  for (size_t i = 0, j = 0; i < len && j < rlen; i++, j += 2) {
    const size_t this = addrs[i]; // the current element

    // 0-length runs are represented by the 0-length range (0, 0)
    if (0 == this) {
      out[j] = out[j + 1] = index_total;

    // n-length runs are the range (x, y) where y-x = n
    } else {
      out[j] = index_total;
      index_total += this;
      out[j + 1] = index_total;
    }
    // needs to be incremented so that the range does not represent nonzero elts
    ++index_total;

  }
  free(addrs);

  return out;
}

/*
  rlep_t* -> rlep_t*

  only the address elements from a sparse array
*/

size_t* _pelements_addr (const rlep_t* const sps) {
  const size_t len = rlep_len(sps);
  size_t* const out = alloc(size_t, len);

  // i = sps; j = out
  for (size_t i = 1, j = 0; j < len; i += 2, j++) {
    //printf("%zu, %zu\n", j, i);
    out[j] = sps[i];
  }

  return out;
}

/*
  rlep_t* -> size_t*

  only the data elements preceded by their real indexes from a sparse array
*/
size_t* _pelements_data (const rlep_t* const sps) {
  const size_t rlen = 2 * rlep_len(sps);
  size_t* const
    out         = alloc(size_t, rlen),
    index_total = 0;

  // i = sps; j = out
  for (size_t i = 2, j = 0; j < rlen; i += 2, j += 2) {
    // the previous element in sps is an address element, so add it
    out[j]     = index_total += sps[i - 1];
    out[j + 1] = sps[i]; // the current data element

    // increment this so ranges work properly
    ++index_total;
  }

  return out;
}

/*
  rlep_t* -> size_t

  number of elements this sparse array is emulating

  essentially values(odd indices) + count(even indices)

  sum of address elements + number of data elements (or the number of address elements)
*/
size_t rlep_len_virtual (const rlep_t* const sps) {

  // address len
  const size_t alen = rlep_len(sps);

  // the number of address elements is also the number of data elements
  size_t sum = alen;

  // every other element is address data
  for (size_t i = 1; i < alen * 2; i += 2) {
    //printf("%" PRIu64 "\n", sps[i]);
    sum += sps[i];
  }

  return sum;
}

/* number of EITHER data or address elements represented by this array */
size_t   rlep_len (const rlep_t* const sps) {
  return sps[0];
}

/* number of actual bytes allocated by this array for primitive indexing */
size_t  rlep_lenr (const rlep_t* const sps) {
  return _real_len(sps[0]);
}

/* algorithm for determining real number of elements */
size_t _real_len (const size_t len) {
  return 1 + (2 * len);
}

/*
  size_t, size_t, size_t -> bool

  python-style range: lower-exclusive, higher-inclusive
*/
bool is_in_range (const size_t n, const size_t low, const size_t high) {
  if ( 0 == (low + high) ) { return false; }
  return n >= low && n < high;
}
