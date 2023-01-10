extern "C" {
#include "ISO_Fortran_binding.h"
}
#include <vector>
#include <array>

extern "C" {
  void ffunc(CFI_cdesc_t* array);
};

int main() {
  std::vector<double> da = {1.0, 2.0, 3.0, 4.0, 5.0};
  CFI_index_t extents[1] = {5};
  CFI_CDESC_T(1) descriptor;

  auto result = CFI_establish(
    (CFI_cdesc_t*) &descriptor,
    da.data(),
    CFI_attribute_pointer,
    CFI_type_double,
    sizeof(double),
    1,
    extents);

  ffunc((CFI_cdesc_t*) &descriptor);
}
