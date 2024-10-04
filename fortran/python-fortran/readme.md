# An example showing calling MPI Fortran code from Python

Here is an example of calling a Fortran code from Python, where the Fortran code can be parallelized using MPI. The Python code will use the `mpi4py` package to handle MPI communication, while the Fortran code will use standard MPI routines for parallel execution.



## Fortran code

This Fortran program approximates the value of pi using the numerical integration of the function $4/(1+x^2)$ over the interval $[0,1]$. It uses MPI to parallelize the computation across multiple processes.



## Steps to Compile and Run



1. **Compile the Fortran code** with MPI enabled:

   ```bash
   mpif90 -o parallel_fortran parallel_fortran.f90
   ```

   

2. **Install the necessary Python package**:

   ```bash
   pip install mpi4py
   ```

   

3. **Run the Python script** to execute the Fortran code using MPI:

   ```
   mpiexec -n 4 python call_fortran.py
   ```

   
