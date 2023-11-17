#define DOPEY_NOT_INCLUDED_DIRECTLY 1
#include "dopey/dope_generated_sizes.h"
#undef DOPEY_NOT_INCLUDED_DIRECTLY

module dope_mod
  use, intrinsic :: iso_c_binding
  implicit none

  private

  public :: dope
  public :: make_dope

  type, bind(c) :: dim_t
    integer(c_int64_t) :: lower_bound
    integer(c_int64_t) :: extent
    integer(c_int64_t) :: sm
  end type

  type, bind(c) :: dope
    type(c_ptr) :: base_addr
    integer(c_size_t) :: elem_len
    integer(c_int64_t) :: rank
    integer(c_int64_t) :: type
    type(dim_t), dimension(DOPEY_DOPE_MAX_RANK) :: dim
  end type

!   interface make_dope_c
! #include "dope_generated_fortran_c_interfaces.h"
!   end interface

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
