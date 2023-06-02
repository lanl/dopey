#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dope_generated_sizes.h"
#undef DOPEY_NOT_INCLUDED_DIRECTLY

module dope_mod
  use, intrinsic :: iso_c_binding
  implicit none

  private

  public :: dope
  public :: make_dope

  integer, parameter :: buffer_size = &
    DOPEY_DOPE_BASE_SIZE + DOPEY_DOPE_PER_DIM_SIZE * DOPEY_DOPE_MAX_RANK

  type, bind(c) :: dope
    character(c_char), dimension(buffer_size) :: buffer
  end type

!   interface make_dope_c
! #include "dope_generated_fortran_c_interfaces.h"
!   end interface

  interface make_dope
#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dope_generated_fortran_interfaces.h"
#undef DOPEY_NOT_INCLUDED_DIRECTLY
  end interface

contains

#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dope_generated_fortran_definitions.h"
#undef DOPEY_NOT_INCLUDED_DIRECTLY

end module
