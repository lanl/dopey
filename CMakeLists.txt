cmake_minimum_required(VERSION 3.14)

set(dopey.is_subproject FALSE)
if( DEFINED PROJECT_NAME )
  set(dopey.is_subproject TRUE)
endif()

cmake_minimum_required(VERSION 3.14)

project(dopey
  LANGUAGES CXX Fortran
  VERSION 0.1.0
)

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

add_subdirectory(src)

if( dopey.testing )
  add_library(dopey_testing INTERFACE)
  target_link_libraries(dopey_testing INTERFACE
    dopey_Fortran dopey_CXX
  )

  find_package(PFUNIT REQUIRED)

  add_subdirectory(test)
endif()

install(TARGETS dopey_Fortran dopey_CXX EXPORT dopeyTargets
  LIBRARY DESTINATION lib
)

install(
  FILES
    src/dopey/dope.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/src/dopey/dope_generated_sizes.h
    ${CMAKE_CURRENT_BINARY_DIR}/src/dopey/dope_generated_type_identifier_traits.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/src/dope_mod.mod
  DESTINATION include/dopey
)

install(EXPORT dopeyTargets
  FILE dopeyTargets.cmake
  NAMESPACE dopey::
  DESTINATION share/cmake/dopey
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(dopeyConfigVersion.cmake
  VERSION ${dopey_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  cmake/dopeyConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/dopeyConfigVersion.cmake
  DESTINATION share/cmake/dopey
)

install(FILES
  cmake/FindISO_Fortran_binding.cmake
  DESTINATION share/cmake/dopey/cmake
)
