add_library(dopey_Fortran)
add_library(dopey::dopey_Fortran ALIAS dopey_Fortran)

target_include_directories(dopey_Fortran
PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/dopey>
)

add_library(dopey_CXX)
add_library(dopey::dopey_CXX ALIAS dopey_CXX)

target_compile_features(dopey_CXX PUBLIC
  cxx_std_14
)

target_include_directories(dopey_CXX PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>
)

target_compile_definitions(dopey_CXX PUBLIC
  $<$<Fortran_COMPILER_ID:Intel>:DOPEY_INTEL_CRAY_BOOL_FIXUP>
  $<$<Fortran_COMPILER_ID:CRAY>:DOPEY_INTEL_CRAY_BOOL_FIXUP>
)

find_package(ISO_Fortran_binding REQUIRED)
target_link_libraries(dopey_CXX PUBLIC
  ISO_Fortran_binding
)

set(prefix $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>$<INSTALL_INTERFACE:include>)
list(APPEND dopey_generated_shared
  ${prefix}/dopey/dope_generated_sizes.h
)

set(prefix $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
list(APPEND dopey_generated_Fortran
  ${prefix}/dopey/dope_generated_fortran_definitions.h
  ${prefix}/dopey/dope_generated_fortran_interfaces.h
)

set(prefix $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
list(APPEND dopey_generated_C
  ${prefix}/dopey/dope_generated_c_definitions.hpp
)

set(prefix $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>$<INSTALL_INTERFACE:include>)
list(APPEND dopey_generated_CXX
  ${prefix}/dopey/dope_generated_type_identifier_traits.hpp
)

find_package(Python REQUIRED COMPONENTS Interpreter)

set(dopey.rank 3 CACHE STRING "Maximum rank of interoperable dope type to generate")
mark_as_advanced(dopey.rank)

set(dopey.types "int;float;double;bool" CACHE STRING "Types interoperable dope type to generate")
mark_as_advanced(dopey.types)

add_custom_command(
  OUTPUT
    "${dopey_generated_shared}"
    "${dopey_generated_Fortran}"
    "${dopey_generated_C}"
    "${dopey_generated_CXX}"
  COMMAND
    ${Python_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/dopey/generate_dope_interfaces.py --outdir ${CMAKE_CURRENT_BINARY_DIR}/dopey ${dopey.rank} ${dopey.types}
  DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/dopey/generate_dope_interfaces.py
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_LIST_DIR}
)

set(prefix $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>$<INSTALL_INTERFACE:include>)
target_sources(dopey_Fortran PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/dopey/dope.F90>
)

foreach(f IN LISTS dopey_generated_Fortran)
  target_sources(dopey_Fortran PRIVATE ${f})
endforeach()

target_sources(dopey_CXX PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/dopey/dope.cpp>
  ${prefix}/dopey/dope.hpp
)

foreach(f IN LISTS dopey_generated_C)
  target_sources(dopey_CXX PRIVATE ${f})
endforeach()

foreach(f IN LISTS dopey_generated_CXX)
  target_sources(dopey_CXX PRIVATE ${f})
endforeach()

foreach(f IN LISTS dopey_generated_shared)
  target_sources(dopey_Fortran PRIVATE ${f})
  target_sources(dopey_CXX PRIVATE ${f})
endforeach()

