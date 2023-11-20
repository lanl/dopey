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
#include <cstddef>
#include <cstring>

namespace dopey {

template<typename T, size_t R>
struct dope { // : cdesc_t {
  using type = T;
  using rank = std::integral_constant<size_t,R>;

  struct dim_t {
    ptrdiff_t lower_bound;
    ptrdiff_t extent;
    ptrdiff_t sm;
  };

  type* base_addr;
  size_t elem_len;
  ptrdiff_t r;
  ptrdiff_t t;
  dim_t dim[DOPEY_DOPE_MAX_RANK];
};

namespace detail {

#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dopey/dope_generated_type_identifier_traits.hpp"
#undef DOPEY_NOT_INCLUDED_DIRECTLY

template<typename T, size_t R
#ifdef DOPEY_INTEL_CRAY_BOOL_FIXUP
         , std::enable_if_t<not std::is_same<T,bool>::value>* = nullptr
#endif
        >
constexpr
bool valid(dope<T,R> const& d) {
  return d.r == R
     and d.t == detail::type_identifier_v<std::remove_cv_t<T>>
     and d.elem_len == sizeof(T);
}

#ifdef DOPEY_INTEL_CRAY_BOOL_FIXUP
/* Cray and Intel set the bool type to CFI_type_signed_char.
 * This may (?) be a bug, but it is ultimately implementation defined */
template<typename T, size_t R,
         std::enable_if_t<std::is_same<T,bool>::value>* = nullptr>
constexpr
bool valid(dope<T,R> const& d) {
  return d.r == R
     and d.t == CFI_type_signed_char
     and d.elem_len == sizeof(T);
}
#endif

} // namespace detail

} // namespace dopey

#endif
