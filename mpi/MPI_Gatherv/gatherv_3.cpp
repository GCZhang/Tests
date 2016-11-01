#include <mpi.h>
#include <vector>
#include <iostream>
#include <stddef.h>
#include <cstdlib>

/*
 *------------------------------------------------------------------------------------
 * 时间：2016-11-01
 * 作者：G.C. Zhang
 * 目的：对Flexible Array Members, MPI_Gatherv and MPI_Type_struct进行测试
 * 描述：使用offsetof()函数和offsets[]数组设置MPI的自定义变量偏移值，故而不必专门处理内
 *       存对齐的问题.
 *       这里使用的技术称为“柔性数组(Flexible Array Members)”，即在struct的最后可以定义
 *       一个空的数组，数组名不占空间。FAM是C99的标准，但是并不是C++的标准。G++目前支持
 *       FAM。
 *       注意FAM的使用和指针类型的转换以及指针的运算。
 * struct { vector; } does not work.
 *------------------------------------------------------------------------------------
*/

struct mesh_flum {
   int face;
   int i;
   int j;
   int k;

   // does not work;
   // std::vector<double> flum;
   
   double flum[0];
};

void print_mesh_flum(int myrank, int vec_len, int LengdreNum, int mesh_flum_bytes, const mesh_flum* mesh_flum_vec){
   std::cout << "CPU " << myrank << ": " << std::endl;
   for (int i = 0; i < vec_len; ++i){
      mesh_flum* scratch = (mesh_flum*)((char*)mesh_flum_vec + i * mesh_flum_bytes); 
      std::cout << scratch->face << " ";
      std::cout << scratch->i << " ";
      std::cout << scratch->j << " ";
      std::cout << scratch->k << " ";
      double *flum = scratch->flum;
      for (int ln = 0; ln < LengdreNum; ++ln)
      {
         std::cout << flum[ln] <<" ";
      }
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
   offsets[4] = offsetof(mesh_flum, flum);
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
   int send_size = myrank + 1;
   
   // Get bytes of one mesh_flum struct object.
   int mesh_flum_bytes = sizeof(mesh_flum) + LengdreNum * sizeof(double);
   mesh_flum* v_send = (mesh_flum*)malloc(send_size * mesh_flum_bytes);
   
   // Set v_send value.
   for (int i = 0; i < send_size; ++i)
   {
      mesh_flum* scratch = (mesh_flum*)((char*)v_send + i * mesh_flum_bytes);
      scratch->face = myrank + 1;
      scratch->i = myrank + 1;
      scratch->j = myrank + 1;
      scratch->k = myrank + 1;
      double *flum = scratch->flum;
      for (int ln = 0; ln < LengdreNum; ++ln)
      {
         flum[ln] = (myrank+1) * 0.1;
      }
   }

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
   /* 只有0号CPU申请缓冲区的值 */
   mesh_flum* v_receive = (mesh_flum*)malloc(sum * mesh_flum_bytes);

   // 搜集操作
   MPI_Gatherv(v_send, send_size, mpi_flum_struct_type, 
      v_receive, &v_counts[0], &v_displs[0],
      mpi_flum_struct_type, root, MPI_COMM_WORLD);

   // 打印
   if (myrank == root){
      print_mesh_flum(myrank, sum, LengdreNum, mesh_flum_bytes, v_receive);
   }
//---------------------------------------------------
   MPI_Finalize();
   return 0;
}
