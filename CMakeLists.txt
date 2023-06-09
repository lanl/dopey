cmake_minimum_required(VERSION 3.14)

set(dopey.is_subproject FALSE)
if( DEFINED PROJECT_NAME )
  set(dopey.is_subproject TRUE)
endif()

cmake_minimum_required(VERSION 3.14)

project(dopey LANGUAGES CXX Fortran)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(CTest)
enable_testing()

include(CMakeDependentOption)
CMAKE_DEPENDENT_OPTION(dopey.testing
  "enable dopoy testing"
  ON
  "NOT ${dopey.is_subproject} AND BUILD_TESTING"
  OFF
)

set(CMAKE_CXX_EXTENSIONS OFF)

add_library(dopey_Fortran)
add_library(dopey::dopey_Fortran ALIAS dopey_Fortran)

target_include_directories(dopey_Fortran
PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>
)

add_library(dopey_CXX)
add_library(dopey::dopey_CXX ALIAS dopey_CXX)

target_compile_features(dopey_CXX PUBLIC
  cxx_std_14
)

target_include_directories(dopey_CXX PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
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

add_subdirectory(src)

if( dopey.testing )
  add_library(dopey_testing INTERFACE)
  target_link_libraries(dopey_testing INTERFACE
    dopey_Fortran dopey_CXX
  )

  find_package(PFUNIT REQUIRED)

  add_subdirectory(test)
endif()
