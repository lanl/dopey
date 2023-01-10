#ifndef DOPEY_DOPE_HPP
#define DOPEY_DOPE_HPP

#include "dope_generated_sizes.h"

extern "C" {
#include <ISO_Fortran_binding.h>
}

#include <cstdlib>
#include <type_traits>
#include <cstring>
#include <cassert>

namespace dopey {

extern "C" {

struct dope_base {
  constexpr static size_t base_size = sizeof(CFI_cdesc_t);
  constexpr static size_t per_rank_size = sizeof(CFI_dim_t);
  constexpr static size_t buffer_size =
    DOPEY_DOPE_BASE_SIZE + DOPEY_DOPE_PER_DIM_SIZE * DOPEY_DOPE_MAX_RANK;
  unsigned char buffer[buffer_size];
};

} // extern "C"

template<typename T, size_t R>
struct dope : dope_base {
  using type = T;
  using rank = std::integral_constant<size_t,R>;
};

template<typename T, size_t R>
CFI_cdesc_t const * to_cdesc(dope<T,R> const& d) {
  CFI_cdesc_t const * const a = static_cast<CFI_cdesc_t const *>(static_cast<void const *>(&d));
  assert(a->rank == R);
  assert(a->elem_len == sizeof(T));
  return a;
}

} // namespace dopey

#endif
