#include <iostream>
#include <stddef.h>
#include <cstdlib>

/* 时间：2016-11-01
 * 作者：G.C. Zhang
 * 目的：测试struct中的FAM以及struct的大小和数据位移。
 * 说明：struct存在内存对齐的现象。注意几种struct的大小和数据的位移。
 *
*/

// 测试FAM
struct test_s{
	int scratch;
	double arr[0];
};

// 只包含一个int的struct
struct pure_int_s
{
	int i;
};

// 一个int和一个double，内存会扩展，sizeof(int_double_s)=16.
struct int_double_s
{
	int i;
	double f;
};

struct mesh_flum {
   int face;
   int i;
   int j;
   int k;

   double flum0;
   double flum1;
   double flum2;
   double flum3;
};

// main
int main(int argc, char const *argv[])
{
	using std::cin;
	using std::cout;
	using std::endl;

	int len = 5;
	int Lengdre =4;
	unsigned int target_bytes = sizeof(test_s) + (Lengdre * sizeof(double));
	test_s *test = (test_s*) malloc(len * target_bytes);

	cout << "sizeof test[0]: " << sizeof(test[0]) << endl;
	cout << "sizeof test_s: " << sizeof(test_s) << endl;

	cout << "sizeof pure_int_s: " << sizeof(pure_int_s) << endl;
	
	cout << "sizeof int_double_s: " << sizeof(int_double_s) << endl;
	cout << "offsetof(int_double_s, i): " << offsetof(int_double_s, i) << endl;
	cout << "offsetof(int_double_s, f): " << offsetof(int_double_s, f) << endl;

	cout << "sizeof mesh_flum: " << sizeof(mesh_flum) << endl;

	cout << "sizeof int: " << sizeof(int) << endl;
	cout << "sizeof double: " << sizeof(double) << endl;
	cout << "target_bytes: " << target_bytes << endl;

	cout << "location of TEST: " << test << endl;
	cout << "TEST + 1: " << test+1 << endl;
	/*cout << "sizeof test[]"*/
	
	for (int i = 0; i < len; ++i)
	{
		test_s* p_current = (test_s*)((char*)test + i * target_bytes);
		p_current->scratch = i;
		double* arr = p_current->arr;
		for (int ln = 0; ln < Lengdre; ++ln)
		{
			arr[ln] = i * 0.1;
		}
	}

	for (int i = 0; i < len; ++i)
	{
		test_s* p_current = (test_s*)((char*)test + i * target_bytes);
		cout << (*p_current).scratch << " ";
		double* arr = p_current->arr;
		for (int ln = 0; ln < Lengdre; ++ln)
		{
			cout << arr[ln] << " ";
		}
		cout << endl;
	}

	return 0;
}
