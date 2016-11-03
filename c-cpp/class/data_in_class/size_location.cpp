#include <iostream>
#include <vector>
#include <stddef.h>

class size_location
{
public:
	int i;
	std::vector<double> arr;

	size_location(){};
	~size_location(){};
	
};

int main(int argc, char const *argv[])
{
	using std::cout; using std::endl;

	size_location size_loc_test;

	cout << "Before resize() operation..." << endl;
	cout << "sizeof size_location: " << sizeof(size_location) << endl;
	cout << "sizeof size_loc_test: " << sizeof(size_loc_test) << endl;
	cout << endl;
	cout << "offsetof(size_location, i): " << offsetof(size_location, i) << endl;
	cout << "offsetof(size_location, arr): " << offsetof(size_location, arr) << endl;
	cout << endl;
	cout << "location of size_loc_test.i: " << &(size_loc_test.i) << endl;
	cout << "location of size_loc_test.arr: " << &(size_loc_test.arr) << endl;

	size_loc_test.arr.resize(10,1.2);
	cout << endl;
	cout << "After resize() operation..." << endl;
	cout << "sizeof size_loc_test: " << sizeof(size_loc_test) << endl;
	cout << "location of size_loc_test.i: " << &(size_loc_test.i) << endl;
	cout << "location of size_loc_test.arr: " << &(size_loc_test.arr) << endl;
	cout << "location of size_loc_test.arr[0]: " << &((size_loc_test.arr)[0]) << endl;
	cout << "location of size_loc_test.arr[1]: " << &((size_loc_test.arr)[1]) << endl;
	cout << "location of size_loc_test.arr.front(): " << &(size_loc_test.arr.front()) << endl;

	return 0;
}