subroutine operations(l, m, n, flag_in)
    implicit none
    
    integer l, m, n
    ! optional argument
    integer, optional :: flag_in

    ! local variable
    integer flag

    ! set default value
    flag = 0

    ! if flag is passed in, then
    if (present(flag_in)) flag = flag_in

    if (flag == 0) then
        l = m + n
    else if (flag == 1) then
        l = m - n
    else if (flag == 2) then
        l = m * n
    else
        write(*,*) "Not implemented.f"
    end if

end subroutine operations

program main
    implicit none

    integer a, b, c
    
    a = 0
    b = 1
    c = 2

    ! default
    call operations(a, b, c)
    write(*,*) "Default: add, a = ", a

    call operations(a, b, c, 1)
    write(*,*) "1: minus, a = ", a

    call operations(a, b, c, 2)
    write(*,*) "2: multiply, a = ", a
    
end program main