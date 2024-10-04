import numpy as np
from mpi4py import MPI
import subprocess

def run_fortran_code():
    # Execute the compiled Fortran program using MPI
    subprocess.run(["mpiexec", "-n", str(MPI.COMM_WORLD.Get_size()), "./parallel_fortran"])

if __name__ == "__main__":
    # Initialize MPI
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()

    # Only process 0 will initiate the Fortran code execution
    if rank == 0:
        run_fortran_code()

    # Finalize MPI in Python
    MPI.Finalize()
