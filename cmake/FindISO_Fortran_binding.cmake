# find parent dir to bin dir
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
