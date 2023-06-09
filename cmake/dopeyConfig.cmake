list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(CMakeFindDependencyMacro)

find_dependency(ISO_Fortran_binding REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/dopeyTargets.cmake)
