#include <mpi.h>
#include <vector>
#include <iostream>

/*
 *------------------------------------------------------------------------------------
 * 时间：2016-10-29
 * 作者：G.C. Zhang
 * 目的：对MPI_Gatherv进行测试
 * 描述：这是一个最基本的MPI_Gatherv的测试。每个进程有一个长度为send_size的数组v_send，
 *       send_size为当前进程号加1，v_send的值为当前的进程号加1。主进程还额外有一个接收
 *       数组v_receive，长度为所有进程上搜集到的数据之和。除此之外，主进程还维护了两个
 *       数组v_counts和v_displs。v_counts记录每个进程发送过来的数据的个数，v_displs记
 *       录每个进程发送过来的数据（即v_send）在缓冲区上（即v_receive）上的位移。
 * 示例：如果以3个进程运行，则进程号为0，1，2。则每个进程上的数据如下：
 *       Process  --  v_send  --  v_counts -- v_displs -- v_receive      
 *          0          [1]         [1,2,3]      [0,1,3]        [1,2,2,3,3,3] 
 *          1          [2,2]         -           -           -
 *          2          [3,3,3]       -           -           -
*/
int main(int argc, char *argv[])
{
   MPI_Init(&argc, &argv);

   using std::cout;
   using std::endl;
   
   int root, myrank, nProc;
   
   std::vector<int> v_send;
   std::vector<int> v_receive;
   std::vector<int> v_counts;
   std::vector<int> v_displs;

   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   MPI_Comm_size(MPI_COMM_WORLD, &nProc);

   int send_size = myrank + 1;
   v_send.resize(send_size, 0);

   if (myrank == 0) v_counts.resize(nProc, 0);
   if (myrank == 0) v_displs.resize(nProc, 0);

   // Gather v_counts[] from all process.
   MPI_Gather(&send_size, 1, MPI_INT,
      &v_counts[0], 1, MPI_INT, root, MPI_COMM_WORLD);

   // Calculate total length of data, e.g. length of v_receive[].
   int sum = 0;
   if (myrank == 0) {
      for (int i = 0; i != v_counts.size(); ++i)
      {
         std::cout << "Counts of process " << i << " is " << v_counts[i] << std::endl;
         sum += v_counts[i];
      }
   }

   // Calculate data displs of each process.
   if (myrank == 0) {
      v_displs[0] = 0;
      for (int i = 1; i != nProc; ++i)
      {
         v_displs[i] = v_displs[i-1] + v_counts[i-1];
         std::cout << "v_displs[" << i << "] is: " << v_displs[i] << std::endl; 
      }
   }

   /* 只有0号CPU申请缓冲区的值 */
   root = 0;
   if (myrank == root)
   {
      v_receive.resize(sum);
   }
   /* 设置v_send的值*/
   for (std::vector<int>::iterator i = v_send.begin(); i != v_send.end(); ++i)
   {
      *i = myrank + 1;
   }

   // 搜集操作
   MPI_Gatherv(&v_send[0], send_size, MPI_INT, 
      &v_receive[0], &v_counts[0], &v_displs[0],
      MPI_INT, root, MPI_COMM_WORLD);

   // 打印
   if (myrank == root){
      for (std::vector<int>::iterator i = v_receive.begin(); i != v_receive.end(); ++i)
      {
         std::cout << *i << " ";
      }
      std::cout << std::endl;
   }

   MPI_Finalize();
   return 0;
}
