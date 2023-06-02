#ifndef DOPEY_DOPE_HPP
#define DOPEY_DOPE_HPP

#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dope_generated_sizes.h"
#undef DOPEY_NOT_INCLUDED_DIRECTLY

#include <cstdio>

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

namespace detail {
#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dope_generated_type_identifier_traits.hpp"
#undef DOPEY_NOT_INCLUDED_DIRECTLY
}

template<typename T, size_t R
#ifdef DOPEY_INTEL_CRAY_BOOL_FIXUP
         , std::enable_if_t<not std::is_same<T,bool>::value>* = nullptr
#endif
        >
CFI_cdesc_t const * to_cdesc(dope<T,R> const& d) {
  CFI_cdesc_t const * const a = static_cast<CFI_cdesc_t const *>(static_cast<void const *>(&d));
  assert(a->rank == R);
  assert(a->type == detail::type_identifier_v<T>);
  assert(a->elem_len == sizeof(T));
  return a;
}

#ifdef DOPEY_INTEL_CRAY_BOOL_FIXUP
/* Cray and Intel set the bool type to CFI_type_signed_char.
 * This may (?) be a bug, but it is ultimately implementation defined */
template<typename T, size_t R,
         std::enable_if_t<std::is_same<T,bool>::value>* = nullptr>
CFI_cdesc_t const * to_cdesc(dope<T,R> const& d) {
  CFI_cdesc_t const * const a = static_cast<CFI_cdesc_t const *>(static_cast<void const *>(&d));
  assert(a->rank == R);
  assert(a->type == CFI_type_signed_char);
  assert(a->elem_len == sizeof(T));
  return a;
}
#endif

} // namespace dopey

#endif
