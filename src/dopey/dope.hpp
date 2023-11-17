#ifndef DOPEY_DOPE_HPP
#define DOPEY_DOPE_HPP

#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dopey/dope_generated_sizes.h"
#undef DOPEY_NOT_INCLUDED_DIRECTLY

extern "C" {
#include <ISO_Fortran_binding.h>
}

#include <utility>
#include <type_traits>

#include <cstdbool>
#include <cstring>
#include <cassert>

namespace dopey {

extern "C" {

struct dim_t {
  ptrdiff_t lower_bound;
  ptrdiff_t extent;
  ptrdiff_t sm;
};

struct cdesc_t {
  void* base_addr;
  size_t elem_len;
  ptrdiff_t rank;
  ptrdiff_t type;
  dim_t dim[DOPEY_DOPE_MAX_RANK];
};

} // extern "C"

template<typename T, size_t R>
struct dope : cdesc_t {
  using type = T;
  using rank = std::integral_constant<size_t,R>;

  template<typename... ArgTs>
  constexpr
  dope(ArgTs&&... args)
  : cdesc_t{std::forward<ArgTs>(args)...}
  {}
};

namespace detail {
#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dopey/dope_generated_type_identifier_traits.hpp"
#undef DOPEY_NOT_INCLUDED_DIRECTLY
}

template<typename T, size_t R
#ifdef DOPEY_INTEL_CRAY_BOOL_FIXUP
         , std::enable_if_t<not std::is_same<T,bool>::value>* = nullptr
#endif
        >
cdesc_t const * to_cdesc(dope<T,R> const& d) {
  cdesc_t const * const a = static_cast<cdesc_t const *>(&d);
  assert(a->rank == R);
  assert(a->type == detail::type_identifier_v<std::remove_cv_t<T>>);
  assert(a->elem_len == sizeof(T));
  return a;
}

#ifdef DOPEY_INTEL_CRAY_BOOL_FIXUP
/* Cray and Intel set the bool type to CFI_type_signed_char.
 * This may (?) be a bug, but it is ultimately implementation defined */
template<typename T, size_t R,
         std::enable_if_t<std::is_same<T,bool>::value>* = nullptr>
cdesc_t const * to_cdesc(dope<T,R> const& d) {
  cdesc_t const * const a = static_cast<cdesc_t const *>(&d);
  assert(a->rank == R);
  assert(a->type == CFI_type_signed_char);
  assert(a->elem_len == sizeof(T));
  return a;
}
#endif

} // namespace dopey

#endif
