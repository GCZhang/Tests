#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    MPI_Group group_global, group_local;
    MPI_Comm  comm_local;
    int my_rank, size, local_size;
    int *process_ranks;

    MPI_Init(0,0);

    MPI_Comm_group(MPI_COMM_WORLD, &group_global);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_size = 4;
    process_ranks = (int*)malloc(local_size*sizeof(int));
    for (int i=0; i != local_size; ++i){
        process_ranks[i] = i;
    }

    MPI_Group_incl(group_global, local_size, process_ranks, &group_local);
    // The following function call will return MPI_COMM_NULL to comm_local on rank 5 - 7.
    MPI_Comm_create(MPI_COMM_WORLD, group_local, &comm_local);

    double x;
    if (my_rank == 0){
        x = 10.0;
    } else {
        x = 1.0;
    }
    if (comm_local != MPI_COMM_NULL) MPI_Bcast(&x, 1, MPI_DOUBLE, 0, comm_local);

    fprintf(stdout, "myrank = %d, x = %f \n", my_rank, x); fflush(stdout);

    MPI_Finalize();
    return 0;
}
