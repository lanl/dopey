module ffunc_mod

contains

subroutine ffunc(array) bind(c)
  use, intrinsic :: iso_c_binding
  implicit none
  real(c_double), dimension(:), pointer :: array

  print *, array
end subroutine

end module ffunc_mod
