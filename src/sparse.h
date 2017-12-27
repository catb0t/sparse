/*

  sparse arrays are hard.
  let's use run-length encoding.
  a run-length encoded sparse array is a primitive C array, not directly connected to
    the object system.

  the following RLEArray has 4 data elements and 5 address elements
  4  0  1  0  2  121  123  188,685,753  188,685,876

  1 and even indexes are address elements, odd indexes are data elements

  4           = number of data elements and the number of other address elements
                represents the usable "length" of the array
                the real number of elements is 1 + (4 * 2) = 9 but it's better than 188m
  0           = distance minus 1! to the next (first) data element index
  1           = the number 1 (at index 0)
  0           = distance minus 1! to the next data element's index
  2           = the number 2 (at index 1)
  120         = one less than the distance to the next data element's index
  123         = the number 123 (at index 122)
  188,685,753 = one less than the distance to the next data
  188,685,876 = the number 188m (at index 188658875)

  it would be presented to the user as a non-sparse array containing at least 188,658,876 elements

  1 2 [121 zeroes] 123 [188m zeroes] 188,658,876

  the advantage of this approach is memory size, and not needing a struct and two
    pointers (which i'm already using everywhere else anyways).

  usually storing 188 million elements in an array takes 8 * 188 million or
    1.5 billion bytes.
  this means that the 'keys' array of a hash table, even if restricted to 100k elements
    can take a lot of memory even if it is very fast.

  i think the disadvantage is that you need a search (rather than a simple indexing)
    to find anything but we'll figure that out when it's implemented.
*/

#ifndef SPARSE_H
#define SPARSE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../deps/yacbnl/yacbnl.min.h"

#ifndef alloc
  #define alloc(type, size) malloc((sizeof (type)) * (size))
#endif

#ifndef set_out_param
  #define set_out_param(name, value) if(NULL!=(name)){*name=value;}
#endif

typedef uint64_t sparse64_t;
typedef bignum_t* sparsebn_t;

sparse64_t*        sparse64_new (const uint64_t* const non_sparse, const size_t len);
sparse64_t*      sparse64_blank (void);
sparse64_t         sparse64_get (const sparse64_t* const sps, const size_t index, bool* const ok);
sparse64_t*     sparse64_insert (const sparse64_t* const sps, const size_t index, const sparse64_t value, bool* const ok);
sparse64_t*     sparse64_delete (const sparse64_t* const sps, const size_t index, bool* const ok);

size_t         sparse64_len (const sparse64_t* const sps);
size_t        sparse64_lenr (const sparse64_t* const sps);
size_t sparse64_len_virtual (const sparse64_t* const sps);

sparse64_t sparse64_search_idx_linear (const sparse64_t* const sps, const size_t index, bool* const ok);
sparse64_t sparse64_search_idx_binary (const sparse64_t* const sps, const size_t index, bool* const ok);

sparse64_t*          sparse64_combine_values(const uint64_t* const data, const size_t* addrs, const size_t data_len, const size_t addrs_len);
sparse64_t*        sparse64_ranges_to_addrs (const size_t* const ranges, const size_t len);
sparse64_t* sparse64_uncompress_zero_ranges (const sparse64_t* const sps);
size_t*       sparse64_compress_zero_ranges (const uint64_t* const nonsparse, const size_t len);

sparse64_t* _elements_addr_64 (const sparse64_t* const sps);
sparse64_t* _elements_data_64 (const sparse64_t* const sps);
size_t           _real_len_64 (const size_t len);
bool           is_in_range_64 (const size_t n, const size_t low, const size_t high);

sparsebn_t*        sparsebn_new (const uint64_t* const non_sparse, const size_t len);
sparsebn_t         sparsebn_get (const sparsebn_t* const sps, const size_t index, bool* const ok);
sparsebn_t*     sparsebn_insert (const sparsebn_t* const sps, const size_t index, const sparsebn_t value, bool* const ok);
sparsebn_t*     sparsebn_delete (const sparsebn_t* const sps, const size_t index, bool* const ok);

sparsebn_t*    _elements_addr_bn (const sparsebn_t* const sps);

/* non-sparse primitive utilities */

uint64_t*              ns64_zero_ranges (const uint64_t* const array, const size_t len);
uint64_t*             ns64_nonzero_elts (const uint64_t* const array, const size_t len);
size_t           ns64_count_next_zeroes (const uint64_t* const array, const size_t len, const size_t idx);
size_t ns64_count_adjacent_nonzero_elts (const uint64_t* const array, const size_t len);
size_t          ns64_count_nonzero_elts (const uint64_t* const array, const size_t len);
size_t           ns64_count_zero_ranges (const uint64_t* const array, const size_t len);
#endif
