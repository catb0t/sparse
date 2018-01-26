#include "../sparse.h"

/*
  sparse64_t* -> size_t

  number of elements this sparse array is emulating

  essentially values(odd indices) + count(even indices)

  sum of address elements + number of data elements (or the number of address elements)
*/
size_t sparse64_len_virtual (const sparse64_t* const sps) {

  // address len
  const size_t alen = sparse64_len_pairs(sps);

  // the number of address elements is also the number of data elements
  size_t sum = alen;

  // every other element is address data
  for (size_t i = 1; i < alen * 2; i += 2) {
    //printf("%" PRIu64 "\n", sps[i]);
    sum += sps[i];
  }

  return sum;
}

/*
  number of EITHER data or address elements represented by this array
  (since there is one address element for each data)
*/
size_t   sparse64_len_pairs (const sparse64_t* const sps) {
  return sps[0];
}

/* number of actual bytes allocated by this array for primitive indexing */
size_t  sparse64_len_real (const sparse64_t* const sps) {
  return _real_len_64(sps[0]);
}

/*
  algorithm for determining real number of data and address elements

  including element 0, the data length element
*/
size_t _real_len_64 (const size_t len) {
  return 1 + (2 * len);
}

/*
  size_t, size_t, size_t -> bool

  python-style range: lower-exclusive, higher-inclusive

  ranges with the same start and end have 0-length and break this rule
    nothing is within a zero length space even though it exists
*/
bool is_in_range_64 (const size_t n, const size_t low, const size_t high) {
  if ( 0 == (low + high) ) { return false; }
  return n >= low && n < high;
}

/*
  determine whether index falls within a zero range (given virtual expansion)
*/
bool sparse64_is_zero_index (const sparse64_t* const sps, const size_t index) {
  const size_t
    vlen = sparse64_len_virtual(sps),
    ranges_len = 2 * sparse64_len_pairs(sps);
  if (index > vlen) {
    return false;
  }

  size_t* const ranges = sparse64_uncompress_zero_ranges(sps);

  for (size_t r = 0; r < ranges_len; r += 2) {
    if ( ranges[r] <= index && ranges[r + 1] >= index ) {
      free(ranges);
      return true;
    }
  }

  free(ranges);
  return false;
}

