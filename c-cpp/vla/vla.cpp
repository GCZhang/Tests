#include <iostream>
#include <stddef.h>
#include <cstdlib>

// test for VLA;
// IS VLA only a GCC extension or C++ Standards?
// Confirmed, G++ extension.

int main(int argc, char const *argv[])
{
   using std::cin;
   using std::cout;
   using std::endl;

   int len;
   cout << "input len:" << endl;
   cin >> len;

   char arr[len];

   arr[0] = 'h';
   arr[1] = 'e';
   arr[2] = 'l';
   arr[3] = 'l';
   arr[4] = 'o';

   for (int i = 0; i < len; ++i)
   {
      cout << arr[i] ;
   }
   cout << endl;

   int arr2[len];
   for (int i = 0; i < len; ++i)
   {
      arr2[i] = 1;
   }
   for (int i = 0; i < len; ++i)
   {
      cout << arr2[i] << " ";
   }
   cout << endl;
   
   return 0;
}
