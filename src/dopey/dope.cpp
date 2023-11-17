#include "dopey/dope.hpp"

#include <cstring>
#include <utility>
#include <cassert>

namespace dopey {

namespace detail {

template<typename T, size_t R, size_t... I>
auto make_dope_impl(CFI_cdesc_t const& a, std::index_sequence<I...>) {
  return dope<T,R>{cdesc_t{a.base_addr, a.elem_len, a.rank, a.type, {dim_t{a.dim[I].lower_bound, a.dim[I].extent, a.dim[I].sm}...}}};
}

template<typename T, size_t R>
auto make_dope(CFI_cdesc_t const& a) {
  return make_dope_impl<T,R>(a, std::make_index_sequence<R>{});
}

} // namespace detail

extern "C" {

#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dopey/dope_generated_c_definitions.hpp"
#undef DOPEY_NOT_INCLUDED_DIRECTLY

} // extern "C"

} // namespaece dopey
