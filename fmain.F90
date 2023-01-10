program main
  use, intrinsic :: iso_fortran_env
  use, intrinsic :: iso_c_binding
  use dope_mod
  implicit none

  integer(int32), dimension(:,:), allocatable, target :: ia
  integer(int32), dimension(:,:), pointer :: ip
  type(dope) :: d

  interface
    subroutine cfunc(array) bind(c)
      use dope_mod
      implicit none
      type(dope) :: array
    end subroutine
  end interface

  allocate(ia(5,5))
  ip => ia
  call cfunc(make_dope(ip(1,:)))
  deallocate(ia)
end program
