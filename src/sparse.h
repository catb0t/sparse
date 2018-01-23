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
  #define set_out_param(name, value) if(NULL!=(name)){*(name)=value;}
#endif

/* an actual number is an atom */
typedef uint64_t sps_atom_t;
/* a pair of these make a descriptor_t */
typedef sps_atom_t* sps_desc_t;
/* an array of these pairs makes a sparse64 array */
typedef sps_desc_t sparse64_t;

sparse64_t*   sparse64_new (const uint64_t* const src, const size_t len);
sparse64_t* sparse64_blank (void);

size_t    sparse64_len_fake (const sparse64_t* const sps);
size_t    sparse64_len_real (const sparse64_t* const sps);
size_t sparse64_len_virtual (const sparse64_t* const sps, bignum_t* const * const out_len);

char*    sparse64_see_real (const sparse64_t* const sps);
char* sparse64_see_virtual (const sparse64_t* const sps, const bool abbr);

#endif
