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
 *  struct { vector; } not work.
 *  [注意struct中的内存对齐，4个整型（4 * 4 bytes）刚好和2个实型(2 * 8bytes)长度一致。编
 *  译器不必再额外为mesh_flum分配内存。否则，还要注意mesh_flum的真实长度和自定义的MPI类
 *  型的长度是否一致。]
 *  事实上，不必担心内存对齐的问题，因为使用了已经offsetof()函数求得了mesh_flum中每一个值
 *  再mesh_flum内部的偏移。并且再构造mpi_flum_struct_type时已经考虑了位移
 *------------------------------------------------------------------------------------
 * TIME : 2016-11-1
 * DEAIL : 将mesh_flum中的实型增加为4个。
*/

struct mesh_flum {
   int face;
   int i;
   int j;
   int k;

   // seems does not work;
   // std::vector<double> flum;
   double flum0;
   double flum1;
   double flum2;
   double flum3;
};


void print_mesh_flum(int myrank, const std::vector<mesh_flum> &mesh_flum_vec){
      std::cout << "CPU " << myrank << ": " << std::endl;
   for (std::vector<mesh_flum>::const_iterator i = mesh_flum_vec.begin(); i != mesh_flum_vec.end(); ++i){
      std::cout << i->face << " ";
      std::cout << i->i << " ";
      std::cout << i->j << " ";
      std::cout << i->k << " ";
      std::cout << i->flum0 << " ";
      std::cout << i->flum1 << " ";
      std::cout << i->flum2 << " ";
      std::cout << i->flum3 << " ";
      std::cout << std::endl;
   }
}

int main(int argc, char *argv[])
{
   MPI_Init(&argc, &argv);

   using std::cout;
   using std::endl;
   
   int root, myrank, nProc;
   root = 0;

   // Get process numbers and my rank id.
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   MPI_Comm_size(MPI_COMM_WORLD, &nProc);

//---------------------------------------------------
   /* create a type for struct mesh_flum. */
   const int nitms = 5;
   int LengdreNum = 4;

   MPI_Datatype mpi_flum_vec_type;
   MPI_Type_contiguous(LengdreNum, MPI_DOUBLE, &mpi_flum_vec_type);
   MPI_Type_commit(&mpi_flum_vec_type);

   int blocklengths[nitms] = {1,1,1,1,1};
   MPI_Datatype types[nitms] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, mpi_flum_vec_type};
   
   MPI_Datatype mpi_flum_struct_type;
   MPI_Aint offsets[nitms];

   offsets[0] = offsetof(mesh_flum, face);
   offsets[1] = offsetof(mesh_flum, i);
   offsets[2] = offsetof(mesh_flum, j);
   offsets[3] = offsetof(mesh_flum, k);
   offsets[4] = offsetof(mesh_flum, flum0);
   if (myrank == 0){
      cout << "offsets: " ;
      for (int i=0; i!=nitms; ++i) {cout << offsets[i] << " ";}
      cout << endl;
   }

   MPI_Type_struct(nitms, blocklengths, offsets, types, &mpi_flum_struct_type);
   MPI_Type_commit(&mpi_flum_struct_type);   
   
//---------------------------------------------------
   /* 设置v_counts, v_displs和v_send */ 
   std::vector<int> v_counts;
   std::vector<int> v_displs;

   if (myrank == 0) v_counts.resize(nProc, 0);
   if (myrank == 0) v_displs.resize(nProc, 0);

   // prepare v_send;
   std::vector<mesh_flum> v_send;
   for (int i = 0; i < myrank+1; ++i)
   {
      v_send.push_back(mesh_flum{myrank+1, myrank+1, myrank+1, myrank+1,
                              (myrank+1)*0.1, (myrank+1)*0.1, (myrank+1)*0.1, (myrank+1)*0.1});
   }

   // get send size of each process.
   int send_size = v_send.size();

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
//---------------------------------------------------
   
   std::vector<mesh_flum> v_receive;
   
   /* 只有0号CPU申请缓冲区的值 */
   if (myrank == root) v_receive.resize(sum);

   // 搜集操作
   MPI_Gatherv(&v_send[0], send_size, mpi_flum_struct_type, 
      &v_receive[0], &v_counts[0], &v_displs[0],
      mpi_flum_struct_type, root, MPI_COMM_WORLD);

   // 打印
   if (myrank == root){
      print_mesh_flum(myrank, v_receive);
   }
//---------------------------------------------------
   MPI_Finalize();
   return 0;
}
