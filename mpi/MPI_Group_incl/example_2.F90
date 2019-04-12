program main
implicit none

#include "mpif.h"

integer :: mpi_comm, mpi_group, mpi_rank, mpi_size
integer :: inc_group, inc_group_size, inc_comm, inc_rank
integer :: group_size
integer :: local_var, global_var
integer :: ierror, i
integer, allocatable :: inc_proc_list(:)

call MPI_Init(ierror)

mpi_comm = MPI_COMM_WORLD

call MPI_COMM_GROUP(mpi_comm, mpi_group, ierror)
call MPI_COMM_RANK(mpi_comm, mpi_rank, ierror)
call MPI_COMM_SIZE(mpi_comm, mpi_size, ierror)

inc_group_size = 1
if (mpi_rank < inc_group_size) then
    allocate(inc_proc_list(inc_group_size))
    do i = 0, inc_group_size-1
        inc_proc_list(i + 1) = i
    end do

    call MPI_GROUP_INCL(mpi_group, inc_group_size, inc_proc_list, inc_group, ierror)
    call MPI_COMM_CREATE(mpi_comm, inc_group, inc_comm, ierror)
else
    !
    allocate(inc_proc_list(mpi_size - inc_group_size))
    do i = 0, size(inc_proc_list) - 1
        inc_proc_list(i + 1) = inc_group_size + i
    end do

    call MPI_GROUP_INCL(mpi_group, size(inc_proc_list), inc_proc_list, inc_group, ierror)
    call MPI_COMM_CREATE(mpi_comm, inc_group, inc_comm, ierror)
end if

call MPI_COMM_RANK(inc_comm, inc_rank, ierror)

local_var = mpi_rank
call MPI_ALLREDUCE(local_var, global_var, 1, MPI_INTEGER, MPI_SUM, inc_comm, ierror)

WRITE(*, *) "my global_rank is", mpi_rank, ", inc_rank is", inc_rank, ", global_var is", global_var, &
            ", inc_comm is", inc_comm, ", global_comm is", mpi_comm 

deallocate(inc_proc_list)
end program main