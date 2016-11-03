#include <iostream>
#include <stddef.h>
#include <cstdlib>
#include <vector>

/* 时间：2016-11-01
 * 作者：G.C. Zhang
 * 目的：测试struct中的FAM以及struct的大小和数据位移。
 *       Class的测试见: ../class/data_in_class/size_location.cpp
 * 说明：struct存在内存对齐的现象。注意几种struct的大小和数据的位移。
 *       int_vec_s的测试说明vector实际并不存储在struct中？sizeof()的
 *       数据也是不对的？
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

// 4 ints and 4 doubles.
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

struct int_vec_s{
	int face;
	std::vector<double> flum;
};

// main
int main(int argc, char const *argv[])
{
	using std::cin;
	using std::cout;
	using std::endl;

	int len = 5;
	int Lengdre =4;

	/*FAM;*/
	unsigned int target_bytes = sizeof(test_s) + (Lengdre * sizeof(double));
	test_s *test = (test_s*) malloc(len * target_bytes);	
	cout << "-----------------------" << endl;
	cout << "sizeof test[0]: " << sizeof(test[0]) << endl;
	cout << "sizeof test_s: " << sizeof(test_s) << endl;
	// 
	cout << "sizeof int: " << sizeof(int) << endl;
	cout << "sizeof double: " << sizeof(double) << endl;
	cout << "target_bytes: " << target_bytes << endl;

	cout << "location of test: " << test << endl;
	cout << "test + 1: " << test+1 << endl;

	cout << "location of test[0].scratch: " << &(test[0].scratch) << endl;
	cout << "location of test[0].arr: " << &(test[0].arr) <<endl;
	cout << "location of test[0].arr[0]: " << &(test[0].arr[0]) << endl;
	cout << "location of test[0].arr[1]: " << &(test[0].arr[1]) << endl;
	cout << "-----------------------" << endl;

	/*pure ints.*/
	cout << "sizeof pure_int_s: " << sizeof(pure_int_s) << endl;
	
	/*int and double struct.*/
	cout << "sizeof int_double_s: " << sizeof(int_double_s) << endl;
	cout << "offsetof(int_double_s, i): " << offsetof(int_double_s, i) << endl;
	cout << "offsetof(int_double_s, f): " << offsetof(int_double_s, f) << endl;

	/*mesh_flum, 4 ints and 4 doubles.*/
	cout << "sizeof mesh_flum: " << sizeof(mesh_flum) << endl;
	cout << "-----------------------" << endl;

	/* int and vector<double> struct */
	int_vec_s int_vec_test;
	cout << "Before resize() operation..." << endl;
	cout << "sizeof int_vec_s: " << sizeof(int_vec_s) << endl;
	cout << "sizeof int_vec_test: " << sizeof(int_vec_test) << endl;
	cout << "offsetof(int_vec, face): " << offsetof(int_vec_s, face) << endl;
	cout << "offsetof(int_vec, flum): " << offsetof(int_vec_s, flum) << endl;
	cout << "sizeof int_vec_test.flum: " << sizeof(int_vec_test.flum) << endl;

	// resize
	int_vec_test.flum.resize(10,0.0);
	cout << "After resize() operation..." << endl;
	cout << "sizeof int_vec_test: " << sizeof(int_vec_test) << endl;
	cout << "sizeof int_vec_test.flum: " << sizeof(int_vec_test.flum) << endl;

	// location of int_vec_test.face and int_vec_test.flum[0], int_vec_test.flum[1]
	cout << "location of int_vec_test.face is: " << &(int_vec_test.face) << endl;
	cout << "location of int_vec_test.flum is: " << &(int_vec_test.flum) << endl;
	cout << "location of int_vec_test.flum[0] is: " << &(int_vec_test.flum[0]) << endl;
	cout << "location of int_vec_test.flum[1] is: " << &(int_vec_test.flum[1]) << endl;
	cout << "location of int_vec_test.flum.front() is: " << &(int_vec_test.flum.front()) << endl;

	cout << endl;
	std::vector<int_double_s> vec_of_int_double;
	vec_of_int_double.push_back(int_double_s{1,1.0});
	vec_of_int_double.push_back(int_double_s{2,2.0});
	cout << "location of vec_of_int_double[0]: " << &(vec_of_int_double[0]) << endl;
	cout << "sizeof vec_of_int_double[0]: " << sizeof(vec_of_int_double[0]) << endl;
	cout << "location of vec_of_int_double[1]: " << &(vec_of_int_double[1]) << endl;
	cout << "sizeof vec_of_int_double[1]: " << sizeof(vec_of_int_double[1]) << endl;
	
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
