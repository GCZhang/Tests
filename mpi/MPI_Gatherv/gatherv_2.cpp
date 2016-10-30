#include <mpi.h>
#include <vector>
#include <iostream>
#include <stddef.h>

/*
 *------------------------------------------------------------------------------------
 * 时间：2016-10-29
 * 作者：G.C. Zhang
 * 目的：对MPI_Gatherv and MPI_Type_struct进行测试
 * 描述：
*/

typedef struct mesh_flum_s {
   int face;
   int i;
   int j;
   int k;
   double flum;
} mesh_flum;

int main(int argc, char *argv[])
{
   MPI_Init(&argc, &argv);

   using std::cout;
   using std::endl;
   
   int root, myrank, nProc;
   root = 0;
   
   std::vector<mesh_flum> v_send;
   std::vector<mesh_flum> v_receive;
   std::vector<int> v_counts;
   std::vector<int> v_displs;

   // Get process numbers and my rank id.
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   MPI_Comm_size(MPI_COMM_WORLD, &nProc);

//---------------------------------------------------
   /* create a type for struct mesh_flum. */
   const int nitms = 5;
   int blocklengths[5] = {1,1,1,1,1};
   MPI_Datatype types[5] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_DOUBLE};
   MPI_Datatype mpi_flum_type;
   MPI_Aint offsets[5];

   offsets[0] = offsetof(mesh_flum, face);
   offsets[1] = offsetof(mesh_flum, i);
   offsets[2] = offsetof(mesh_flum, j);
   offsets[3] = offsetof(mesh_flum, k);
   offsets[4] = offsetof(mesh_flum, flum);
   /*for (int i=0; i!=5; ++i) {cout << "offsets: " << offsets[i] << " ";}*/

   MPI_Type_struct(nitms, blocklengths, offsets, types, &mpi_flum_type);
   MPI_Type_commit(&mpi_flum_type);   
   
//---------------------------------------------------
   int send_size = myrank + 1;
   v_send.resize(send_size);

   if (myrank == 0) v_counts.resize(nProc, 0);
   if (myrank == 0) v_displs.resize(nProc, 0);

   // Gather v_counts[] from all process.
   MPI_Gather(&send_size, 1, MPI_INT,
      &v_counts[0], 1, MPI_INT, root, MPI_COMM_WORLD);

   // Calculate total length of data, e.g. length of v_receive[].
   int sum = 0;
   if (myrank == 0) {
      for (int i = 0; i != v_counts.size(); ++i){
         std::cout << "Counts of process " << i << " is " << v_counts[i] << std::endl;
         sum += v_counts[i];
      }
   }

   // Calculate data displs of each process.
   if (myrank == 0) {
      v_displs[0] = 0;
      for (int i = 1; i != nProc; ++i){
         v_displs[i] = v_displs[i-1] + v_counts[i-1];
         std::cout << "v_displs[" << i << "] is: " << v_displs[i] << std::endl; 
      }
   }

   /* 只有0号CPU申请缓冲区的值 */
   if (myrank == root) v_receive.resize(sum);

   /* 设置v_send的值*/
   for (std::vector<mesh_flum>::iterator i = v_send.begin(); i != v_send.end(); ++i){
      (*i).face = myrank + 1;
      (*i).i = myrank + 1;
      (*i).j = myrank + 1;
      (*i).k = myrank + 1;
      (*i).flum = myrank;
   }

//---------------------------------------------------

   // 搜集操作
   MPI_Gatherv(&v_send[0], send_size, mpi_flum_type, 
      &v_receive[0], &v_counts[0], &v_displs[0],
      mpi_flum_type, root, MPI_COMM_WORLD);

   // 打印
   if (myrank == root){
      for (std::vector<mesh_flum>::iterator i = v_receive.begin(); i != v_receive.end(); ++i){
         std::cout << (*i).face << " ";
         std::cout << (*i).i << " ";
         std::cout << (*i).j << " ";
         std::cout << (*i).k << " ";
         std::cout << (*i).flum << " ";
         std::cout << std::endl;
      }
      std::cout << std::endl;
   }

   MPI_Finalize();
   return 0;
}
