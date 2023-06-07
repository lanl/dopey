# This is a hack to work around compiler wrappers (e.g., Cray and MPI)
# The search for the ISO_Fortran_binding.h header needs the actual compiler path and not a wrapper path.
# If a wrapper is being used, this hack finds the actual compiler path.
if( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL Cray )
  find_program(CRAY_Fortran_COMPILER "crayftn")
  set(CMAKE_Fortran_COMPILER ${CRAY_Fortran_COMPILER})
elseif( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL Intel )
  find_program(CRAY_Fortran_COMPILER "ifort")
  set(CMAKE_Fortran_COMPILER ${CRAY_Fortran_COMPILER})
elseif( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL GNU )
  find_program(CRAY_Fortran_COMPILER "gfortran")
  set(CMAKE_Fortran_COMPILER ${CRAY_Fortran_COMPILER})
elseif( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL XL )
  find_program(CRAY_Fortran_COMPILER "xlf")
  set(CMAKE_Fortran_COMPILER ${CRAY_Fortran_COMPILER})
endif()

get_filename_component(Fortran_compiler_dir ${CMAKE_Fortran_COMPILER} DIRECTORY)
get_filename_component(Fortran_compiler_base ${Fortran_compiler_dir} NAME)
while(NOT "${Fortran_compiler_base}" STREQUAL "bin")
  get_filename_component(Fortran_compiler_dir ${Fortran_compiler_dir} DIRECTORY)
  get_filename_component(Fortran_compiler_base ${Fortran_compiler_dir} NAME)
endwhile()
get_filename_component(Fortran_compiler_dir ${Fortran_compiler_dir} DIRECTORY)

find_file(ISO_Fortran_binding_PATH
	NAMES ISO_Fortran_binding.h
	PATHS ${Fortran_compiler_dir}
			  ${Fortran_compiler_dir}/*
			  ${Fortran_compiler_dir}/*/*
			  ${Fortran_compiler_dir}/*/*/*
			  ${Fortran_compiler_dir}/*/*/*/*
			  ${Fortran_compiler_dir}/*/*/*/*/*
	PATH_SUFFIXES include
	NO_DEFAULT_PATHS)

if( ISO_Fortran_binding_PATH )
  get_filename_component(ISO_Fortran_binding_INCLUDE_DIR ${ISO_Fortran_binding_PATH} DIRECTORY)
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
  ISO_Fortran_binding DEFAULT_MSG ISO_Fortran_binding_INCLUDE_DIR)

if( ISO_Fortran_binding_FOUND )
  add_library(ISO_Fortran_binding INTERFACE IMPORTED)
  set_target_properties(ISO_Fortran_binding
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ISO_Fortran_binding_INCLUDE_DIR})
endif()
