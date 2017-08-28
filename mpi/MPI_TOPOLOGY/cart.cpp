#include <mpi.h>
#include <iostream>

int main(int args, char** argv){
    int Size;
    int Rank;
    bool Reorder = false;
    int nDims = 3;
    int Rank_coords;
    int Local_Rank_G;
    int Local_Rank_A;
    int Local_Rank_S;

    MPI_Comm cartComm;
    MPI_Comm Comm_G;
    MPI_Comm Comm_A;
    MPI_Comm Comm_S;

    MPI_Group Group_G, Group_A, Group_S;

    int coords[3];

    MPI_Init(&args, &argv);

    int dims[3] = {2,2,2};
    int perd[3] = {0,0,0};

    //-------------------------------------------------------------------
    // Get global size and rank
    MPI_Comm_size(MPI_COMM_WORLD, &Size);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    // Create Cartesian topology.
    // Cartesian coordinates indices start from 0 and use row-major layout.
    MPI_Cart_create(MPI_COMM_WORLD, nDims, dims, perd, Reorder, &cartComm);
    MPI_Cart_coords(cartComm, Rank, nDims, coords);
    MPI_Comm_rank(cartComm, &Rank_coords);

    // std::cout << "My global rank is " << Rank << ". Rank in cartComm is: "
    //     << Rank_coords << ". coords is: " << coords[0] << " " << coords[1] 
    //     << " " << coords[2] << std::endl;
    // flush(std::cout);

    MPI_Barrier(cartComm);
    if (Rank == 0) std:: cout << std::endl;

    //--------------------------------------------------------------------
    // Create sub communicators
    int Remain[3] = {1, 0, 0};
    MPI_Cart_sub(cartComm, Remain, &Comm_G);
    MPI_Comm_group(Comm_G, &Group_G);

    Remain[0] = 0; Remain[1] = 1; Remain[2] = 0;
    MPI_Cart_sub(cartComm, Remain, &Comm_A);
    MPI_Comm_group(Comm_A, &Group_A);

    Remain[0] = 0; Remain[1] = 0; Remain[2] = 1;
    MPI_Cart_sub(cartComm, Remain, &Comm_S);
    MPI_Comm_group(Comm_S, &Group_S);

    // std::cout << "My global rank is " << Rank << ". Rank in cartComm is: "
    //     << Rank_coords << ". coords is: " << coords[0] << " " << coords[1] 
    //     << " " << coords[2] << ". Group_G is: " << Group_G << ". Group_A is: "
    //     << Group_A << ". Group_S is: "<< Group_S << std::endl;
    // std::cout << "My global rank is " << Rank << ". Rank in cartComm is: "
    //     << Rank_coords << ". coords is: " << coords[0] << " " << coords[1] 
    //     << " " << coords[2] << ". Comm_G is: " << Comm_G << ". Comm_A is: "
    //     << Comm_A << ". Comm_S is: "<< Comm_S << std::endl;
    //  flush(std::cout);
    //--------------------------------------------------------------------
    int local = Rank;
    int Reduce_G = 0, Reduce_A = 0, Reduce_S = 0;
    
    MPI_Allreduce(&local, &Reduce_G, 1, MPI_INT, MPI_SUM, Comm_G);
    MPI_Allreduce(&local, &Reduce_A, 1, MPI_INT, MPI_SUM, Comm_A);
    MPI_Allreduce(&local, &Reduce_S, 1, MPI_INT, MPI_SUM, Comm_S);

    std::cout << "My global rank is " << Rank << ". coords is: " 
        << coords[0] << " " << coords[1] << " " << coords[2]
        << ". Reduce_G is: " << Reduce_G
        << ". Reduce_A is: " << Reduce_A
        << ". Reduce_S is: " << Reduce_S
        << std::endl;
    flush(std::cout);

    MPI::Finalize();

}

