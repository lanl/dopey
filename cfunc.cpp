#include "dope.hpp"

#include <cstdio>

extern "C" {

void cfunc(dopey::dope<bool,1> const& d) {
  printf("sizeof(CFI_cdesc_t) = %ld\n", sizeof(CFI_cdesc_t));
  printf("sizeof(CFI_dim_t) = %ld\n", sizeof(CFI_dim_t));
  CFI_cdesc_t const * array = dopey::to_cdesc(d);

  CFI_rank_t rank = array->rank;
  CFI_type_t type = array->type;
  size_t elem_len = array->elem_len;
  void* base_addr = array->base_addr;

  printf("rank = %td\n", rank);
  printf("type = %td\n", type);
  printf("elem_len = %ld\n", elem_len);
  printf("base_addr = %p\n", base_addr);

  for(CFI_rank_t i=0; i<rank; ++i) {
    CFI_dim_t const* dim = &array->dim[i];
    printf("lower_bound[%td] = %ld\n", i, dim->lower_bound);
    printf("extent[%td] = %ld\n", i, dim->extent);
    printf("sm[%td] = %ld\n", i, dim->sm);
  }
}

} // extern "C"
