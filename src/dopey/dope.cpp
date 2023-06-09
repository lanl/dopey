#include "dopey/dope.hpp"

#include <cstring>
#include <cassert>

namespace dopey {

extern "C" {

#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dopey/dope_generated_c_definitions.hpp"
#undef DOPEY_NOT_INCLUDED_DIRECTLY

#undef DOPEY_INCLUDED_FROM_DOPE_CPP

} // extern "C"

} // namespaece dopey
