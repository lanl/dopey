add_library(f_to_c_CXX
  verify_functions.cpp
)

target_link_libraries(f_to_c_CXX PRIVATE
  dopey_testing
)

add_pfunit_ctest(f_to_c
  TEST_SOURCES f_to_c.pf
  LINK_LIBRARIES f_to_c_CXX dopey_testing
)

if( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL Intel )
  set_target_properties(f_to_c PROPERTIES
    LINKER_LANGUAGE Fortran
  )
endif()
