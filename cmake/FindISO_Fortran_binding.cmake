# This is a hack to work around compiler wrappers (e.g., Cray and MPI)
# The search for the ISO_Fortran_binding.h header needs the actual compiler path and not a wrapper path.
# If a wrapper is being used, this hack finds the actual compiler path.
if( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL Cray )
  find_program(ISO_Fortran_binding_COMPILER "crayftn")
elseif( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL Intel )
  find_program(ISO_Fortran_binding_COMPILER "ifort")
elseif( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL GNU )
  find_program(ISO_Fortran_binding_COMPILER "gfortran")
elseif( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL XL )
  find_program(ISO_Fortran_binding_COMPILER "xlf")
endif()

get_filename_component(ISO_Fortran_binding_compiler_dir ${ISO_Fortran_binding_COMPILER} DIRECTORY)
get_filename_component(ISO_Fortran_binding_compiler_base ${ISO_Fortran_binding_compiler_dir} NAME)
while(NOT "${ISO_Fortran_binding_compiler_base}" STREQUAL "bin")
  get_filename_component(ISO_Fortran_binding_compiler_dir ${ISO_Fortran_binding_compiler_dir} DIRECTORY)
  get_filename_component(ISO_Fortran_binding_compiler_base ${ISO_Fortran_binding_compiler_dir} NAME)
endwhile()
get_filename_component(ISO_Fortran_binding_compiler_dir ${ISO_Fortran_binding_compiler_dir} DIRECTORY)

find_file(ISO_Fortran_binding_PATH
	NAMES ISO_Fortran_binding.h
	PATHS ${ISO_Fortran_binding_compiler_dir}
			  ${ISO_Fortran_binding_compiler_dir}/*
			  ${ISO_Fortran_binding_compiler_dir}/*/*
			  ${ISO_Fortran_binding_compiler_dir}/*/*/*
			  ${ISO_Fortran_binding_compiler_dir}/*/*/*/*
			  ${ISO_Fortran_binding_compiler_dir}/*/*/*/*/*
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
