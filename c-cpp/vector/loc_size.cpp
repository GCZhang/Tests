#include <vector>
#include <iostream>
#include <stddef.h>

int main(int argc, char const *argv[])
{

	using std::cout;
	using std::endl;

	std::vector<std::vector<double> > v_flum;
	std::vector<double> v1 = {1.0,1.0,1.0,1.0};
	std::vector<double> v2 = {1.0,1.0,1.0,1.0};

	cout << "Before push_back: " << endl;
	cout << "sizeof v_flum: " << sizeof(v_flum) << endl;
	cout << "Location of v_flum: " << &(v_flum) << endl;
	
	cout << "-------------------" << endl;
	cout << "After push_back: " << endl;
	v_flum.push_back(v1);
	v_flum.push_back(v2);
	cout << "sizeof v_flum: " << sizeof(v_flum) << endl;
	cout << "Location of v_flum: " << &(v_flum) << endl;
	cout << "sizeof v_flum[0]: " << sizeof(v_flum[0]) << endl;
	cout << "Location of v_flum[0]: " << &(v_flum[0]) << endl;
	cout << "sizeof v_flum[1]: " << sizeof(v_flum[1]) << endl;
	cout << "Location of v_flum[1]: " << &(v_flum[1]) << endl;
	cout << "&(v_flum[1]) - &(v_flum[0]): " << &(v_flum[1]) - &(v_flum[0]) << endl;
	cout << "sizeof v_flum[0][0]: " << sizeof(v_flum[0][0]) << endl;
	cout << "Location v_flum[0][0]: " << &(v_flum[0][0]) << endl;
	cout << "Location v_flum[0][1]: " << &(v_flum[0][1]) << endl;
	cout << "Location v_flum[0][2]: " << &(v_flum[0][2]) << endl;
	cout << "Location v_flum[0][3]: " << &(v_flum[0][3]) << endl;
	cout << "Location v_flum[1][0]: " << &(v_flum[1][0]) << endl;
	cout << "Location v_flum[1][1]: " << &(v_flum[1][1]) << endl;
	cout << "Location v_flum[1][2]: " << &(v_flum[1][2]) << endl;
	cout << "Location v_flum[1][3]: " << &(v_flum[1][3]) << endl;

	cout << endl;
	cout << "sizeof v1: " << sizeof(v1) << endl;
	cout << "Location of v1: " << &v1 << endl;
	cout << "Location of v1[0]: " << &(v1[0]) << endl;

	double a = 1.0;
	double* p_double = &a;
	cout << endl;
	cout << "value of pointer to double: " << p_double << endl;
	cout << "sizeof pointer to double: " << sizeof(p_double) << endl;

	return 0;
}