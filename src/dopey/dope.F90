module dope_mod
  use, intrinsic :: iso_c_binding
  implicit none

  private

  public :: make_dope

  type, bind(c) :: dim_t
    integer(c_int64_t) :: lower_bound
    integer(c_int64_t) :: extent
    integer(c_int64_t) :: sm
  end type

#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dopey/dope_generated_fortran_types.h"
#undef DOPEY_NOT_INCLUDED_DIRECTLY

  interface make_dope
#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dopey/dope_generated_fortran_interfaces.h"
#undef DOPEY_NOT_INCLUDED_DIRECTLY
  end interface

contains

#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dopey/dope_generated_fortran_definitions.h"
#undef DOPEY_NOT_INCLUDED_DIRECTLY

end module
