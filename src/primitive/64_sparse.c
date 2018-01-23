#include "../sparse.h"


sparse64_t* sparse64_new (const uint64_t* const src, const size_t len) {
  return NULL;
}

sparse64_t* sparse64_blank (void) {
  const size_t atom_pair_len = 2;

  sparse64_t* out = alloc(sparse64_t, 1);

  out[0] = memset( alloc(sps_atom_t, atom_pair_len) , 0, atom_pair_len);

  return out;
}

