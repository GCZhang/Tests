program mpi_parallel_fortran
    use mpi
    implicit none
    integer :: ierr, rank, size, i
    integer, parameter :: n = 1000000
    real(8) :: local_sum, global_sum, pi, dx, x

    ! Initialize MPI
    call MPI_Init(ierr)
    call MPI_Comm_rank(MPI_COMM_WORLD, rank, ierr)
    call MPI_Comm_size(MPI_COMM_WORLD, size, ierr)

    dx = 1.0d0 / n
    local_sum = 0.0d0

    ! Each process computes a portion of the sum
    do i = rank + 1, n, size
        x = (i - 0.5d0) * dx
        local_sum = local_sum + 4.0d0 / (1.0d0 + x**2)
    end do

    local_sum = local_sum * dx

    ! Reduce the results to get the global sum (total pi approximation)
    call MPI_Reduce(local_sum, global_sum, 1, MPI_DOUBLE_PRECISION, MPI_SUM, 0, MPI_COMM_WORLD, ierr)

    ! Process with rank 0 prints the result
    if (rank == 0) then
        pi = global_sum
        print *, "Approximated Pi value: ", pi
    end if

    ! Finalize MPI
    call MPI_Finalize(ierr)
end program mpi_parallel_fortran
