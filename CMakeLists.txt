cmake_minimum_required(VERSION 3.14)

project(c_f_test LANGUAGES CXX Fortran)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(ISO_Fortran_binding REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_library(c_f_test_CXX cfunc.cpp dope.cpp dope.hpp)
target_link_libraries(c_f_test_CXX PRIVATE ISO_Fortran_binding)
target_compile_definitions(c_f_test_CXX PRIVATE
  $<$<Fortran_COMPILER_ID:Intel>:DOPEY_INTEL_CRAY_BOOL_FIXUP>
  $<$<Fortran_COMPILER_ID:Cray>:DOPEY_INTEL_CRAY_BOOL_FIXUP>
)
add_library(c_f_test_Fortran ffunc.F90 dope.F90)

add_executable(c_f_test_fmain)
target_sources(c_f_test_fmain PRIVATE fmain.F90)
target_link_libraries(c_f_test_fmain PRIVATE c_f_test_Fortran c_f_test_CXX)

if( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL Intel
 OR "${CMAKE_Fortran_COMPILER_ID}" STREQUAL IntelLLVM
 OR "${CMAKE_Fortran_COMPILER_ID}" STREQUAL Cray)
set_target_properties(c_f_test_fmain
  PROPERTIES LINKER_LANGUAGE Fortran
)
endif()

add_executable(c_f_test_cmain)
target_sources(c_f_test_cmain PRIVATE cmain.cpp)
target_link_libraries(c_f_test_cmain PRIVATE c_f_test_Fortran c_f_test_CXX ISO_Fortran_binding)

if("${CMAKE_Fortran_COMPILER_ID}" STREQUAL Cray)
set_target_properties(c_f_test_cmain
  PROPERTIES LINKER_LANGUAGE Fortran
)
endif()
