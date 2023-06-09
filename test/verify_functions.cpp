#include "dopey/dope.hpp"

extern "C" {

bool verify_int_1d_array_c(dopey::dope<int,1>& d) {
  auto cdesc = to_cdesc(d);

  auto stride0 = cdesc->dim[0].sm / cdesc->elem_len;

  bool result = true;
  for(auto i=cdesc->dim[0].lower_bound-1; i<cdesc->dim[0].extent; ++i) {
    result &= *(static_cast<int*>(cdesc->base_addr) + stride0*i) == (i+1);
  }

  return result;
}

bool verify_int_2d_array_c(dopey::dope<int,2>& d) {
  auto cdesc = to_cdesc(d);

  auto stride0 = cdesc->dim[0].sm / cdesc->elem_len;
  auto stride1 = cdesc->dim[1].sm / cdesc->elem_len;

  bool result = true;
  for(auto i=cdesc->dim[0].lower_bound-1; i<cdesc->dim[0].extent; ++i) {
    for(auto j=cdesc->dim[1].lower_bound-1; j<cdesc->dim[1].extent; ++j) {
      result &= *(static_cast<int*>(cdesc->base_addr) + stride0*i +stride1*j)
                == (i+1)*(j+1);
    }
  }

  return result;
}

bool verify_int_3d_array_c(dopey::dope<int,3>& d) {
  auto cdesc = to_cdesc(d);

  auto stride0 = cdesc->dim[0].sm / cdesc->elem_len;
  auto stride1 = cdesc->dim[1].sm / cdesc->elem_len;
  auto stride2 = cdesc->dim[2].sm / cdesc->elem_len;

  bool result = true;
  for(auto i=cdesc->dim[0].lower_bound-1; i<cdesc->dim[0].extent; ++i) {
    for(auto j=cdesc->dim[1].lower_bound-1; j<cdesc->dim[1].extent; ++j) {
      for(auto k=cdesc->dim[2].lower_bound-1; k<cdesc->dim[2].extent; ++k) {
        result &= *(static_cast<int*>(cdesc->base_addr)
                                        + stride0*i + stride1*j + stride2*k)
                  == (i+1) * (j+1) * (k+1);
      }
    }
  }

  return result;
}

bool verify_float_1d_array_c(dopey::dope<float,1>& d) {
  auto cdesc = to_cdesc(d);

  auto stride0 = cdesc->dim[0].sm / cdesc->elem_len;

  bool result = true;
  for(auto i=cdesc->dim[0].lower_bound-1; i<cdesc->dim[0].extent; ++i) {
    result &= *(static_cast<float*>(cdesc->base_addr) + stride0*i) == (i+1);
  }

  return result;
}

bool verify_double_1d_array_c(dopey::dope<double,1>& d) {
  auto cdesc = to_cdesc(d);

  auto stride0 = cdesc->dim[0].sm / cdesc->elem_len;

  bool result = true;
  for(auto i=cdesc->dim[0].lower_bound-1; i<cdesc->dim[0].extent; ++i) {
    result &= *(static_cast<double*>(cdesc->base_addr) + stride0*i) == (i+1);
  }

  return result;
}

bool verify_bool_1d_array_c(dopey::dope<bool,1>& d) {
  auto cdesc = to_cdesc(d);

  auto stride0 = cdesc->dim[0].sm / cdesc->elem_len;

  bool result = true;
  for(auto i=cdesc->dim[0].lower_bound-1; i<cdesc->dim[0].extent; ++i) {
    if( i % 2 == 0) {
      result &= not *(static_cast<bool*>(cdesc->base_addr) + stride0*i);
    }
    else {
      result &= *(static_cast<bool*>(cdesc->base_addr) + stride0*i);
    }
  }

  return result;
}

} // extern "C"
