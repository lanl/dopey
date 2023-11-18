#include "dopey/dope.hpp"

extern "C" {

bool verify_int_0d_array_c(dopey::dope<int,0>& d) {
  bool retval = *(static_cast<int*>(d.base_addr)) == 5;
  *(static_cast<int*>(d.base_addr)) = 25;
  return retval;
}

bool verify_float_0d_array_c(dopey::dope<float,0>& d) {
  bool retval = *(static_cast<float*>(d.base_addr)) == 5.0;
  *(static_cast<float*>(d.base_addr)) = 3.14;
  return retval;
}

bool verify_double_0d_array_c(dopey::dope<double,0>& d) {
  bool retval = *(static_cast<double*>(d.base_addr)) == 5.0;
  *(static_cast<double*>(d.base_addr)) = 3.14;
  return retval;
}

bool verify_bool_0d_array_c(dopey::dope<bool,0>& d) {
  bool retval = *(static_cast<bool*>(d.base_addr)) == false;
  *(static_cast<bool*>(d.base_addr)) = true;
  return retval;
}

bool verify_int_1d_array_c(dopey::dope<int,1>& d) {
  auto stride0 = d.dim[0].sm / d.elem_len;

  bool result = true;
  for(auto i=d.dim[0].lower_bound-1; i<d.dim[0].extent; ++i) {
    result &= *(static_cast<int*>(d.base_addr) + stride0*i) == (i+1);
  }

  return result;
}

bool verify_int_2d_array_c(dopey::dope<int,2>& d) {
  auto stride0 = d.dim[0].sm / d.elem_len;
  auto stride1 = d.dim[1].sm / d.elem_len;

  bool result = true;
  for(auto i=d.dim[0].lower_bound-1; i<d.dim[0].extent; ++i) {
    for(auto j=d.dim[1].lower_bound-1; j<d.dim[1].extent; ++j) {
      result &= *(static_cast<int*>(d.base_addr) + stride0*i +stride1*j)
                == (i+1)*(j+1);
    }
  }

  return result;
}

bool verify_int_3d_array_c(dopey::dope<int,3>& d) {
  auto cdesc = to_cdesc(d);

  auto stride0 = d.dim[0].sm / d.elem_len;
  auto stride1 = d.dim[1].sm / d.elem_len;
  auto stride2 = d.dim[2].sm / d.elem_len;

  bool result = true;
  for(auto i=d.dim[0].lower_bound-1; i<d.dim[0].extent; ++i) {
    for(auto j=d.dim[1].lower_bound-1; j<d.dim[1].extent; ++j) {
      for(auto k=d.dim[2].lower_bound-1; k<d.dim[2].extent; ++k) {
        result &= *(static_cast<int*>(d.base_addr)
                                        + stride0*i + stride1*j + stride2*k)
                  == (i+1) * (j+1) * (k+1);
      }
    }
  }

  return result;
}

bool verify_float_1d_array_c(dopey::dope<float,1>& d) {
  auto cdesc = to_cdesc(d);

  auto stride0 = d.dim[0].sm / d.elem_len;

  bool result = true;
  for(auto i=d.dim[0].lower_bound-1; i<d.dim[0].extent; ++i) {
    result &= *(static_cast<float*>(d.base_addr) + stride0*i) == (i+1);
  }

  return result;
}

bool verify_double_1d_array_c(dopey::dope<double,1>& d) {
  auto cdesc = to_cdesc(d);

  auto stride0 = d.dim[0].sm / d.elem_len;

  bool result = true;
  for(auto i=d.dim[0].lower_bound-1; i<d.dim[0].extent; ++i) {
    result &= *(static_cast<double*>(d.base_addr) + stride0*i) == (i+1);
  }

  return result;
}

bool verify_bool_1d_array_c(dopey::dope<bool,1>& d) {
  auto cdesc = to_cdesc(d);

  auto stride0 = d.dim[0].sm / d.elem_len;

  bool result = true;
  for(auto i=d.dim[0].lower_bound-1; i<d.dim[0].extent; ++i) {
    if( i % 2 == 0) {
      result &= not *(static_cast<bool*>(d.base_addr) + stride0*i);
    }
    else {
      result &= *(static_cast<bool*>(d.base_addr) + stride0*i);
    }
  }

  return result;
}

} // extern "C"
