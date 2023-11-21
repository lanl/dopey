#ifndef DOPEY_DOPE_HPP
#define DOPEY_DOPE_HPP

extern "C" {
#include <ISO_Fortran_binding.h>
}

#include <utility>
#include <type_traits>

#include <cstdbool>
#include <cstddef>
#include <cstring>

#include <array>

namespace dopey {

namespace detail {

template<typename T>
struct dope_base {
  T* base_addr;
  size_t elem_len;
  ptrdiff_t r;
  ptrdiff_t t;
};

} // namespace detail

template<typename T, size_t R>
struct dope : detail::dope_base<T> {
  using type = T;
  using rank = std::integral_constant<size_t,R>;

  struct dim_t {
    ptrdiff_t lower_bound;
    ptrdiff_t extent;
    ptrdiff_t sm;
  };

  dim_t dim[R];

  template<typename... DimTs>
  constexpr
  dope(T* base_addr_, size_t elem_len_, ptrdiff_t r_, ptrdiff_t t_, DimTs... dim_)
  : detail::dope_base<T>{base_addr_, elem_len_, r_, t_}
  , dim{dim_...}
  {}
};

template<typename T>
struct dope<T,0> : detail::dope_base<T> {
  using type = T;
  using rank = std::integral_constant<size_t,0>;

  constexpr
  dope(T* base_addr_, size_t elem_len_, ptrdiff_t r_, ptrdiff_t t_)
  : detail::dope_base<T>{base_addr_, elem_len_, r_, t_}
  {}
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
bool valid(dope<T,R> const& d, CFI_cdesc_t const& a) {
  return a.rank == R
     and a.type == detail::type_identifier_v<std::remove_cv_t<T>>
     and a.elem_len == sizeof(T);
}

#ifdef DOPEY_INTEL_CRAY_BOOL_FIXUP
/* Cray and Intel set the bool type to CFI_type_signed_char.
 * This may (?) be a bug, but it is ultimately implementation defined */
template<typename T, size_t R,
         std::enable_if_t<std::is_same<T,bool>::value>* = nullptr>
constexpr
bool valid(dope<T,R> const& d, CFI_cdesc_t const& a) {
  return a.rank == R
     and a.type == CFI_type_signed_char
     and a.elem_len == sizeof(T);
}
#endif

} // namespace detail

} // namespace dopey

#endif
