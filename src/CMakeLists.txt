set(prefix $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>$<INSTALL_INTERFACE:include>)

target_sources(dopey_Fortran PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/dopey/dope.F90>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/dopey/dope_generated_fortran_definitions.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/dopey/dope_generated_fortran_interfaces.h>
  ${prefix}/dopey/dope_generated_sizes.h
)

target_sources(dopey_CXX PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/dopey/dope.cpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/dopey/dope_generated_c_definitions.hpp>
  ${prefix}/dopey/dope_generated_sizes.h
  ${prefix}/dopey/dope.hpp
  ${prefix}/dopey/dope_generated_type_identifier_traits.hpp
)
