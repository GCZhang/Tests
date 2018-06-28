#include "Vec.h"
#include "VecExpression.h"
#include "VecSum.h"
#include <iostream>

int main (int args, char** argv)
{
    Vec v0 = {1.0, 2.0, 3.0, 4.0};
    Vec v1 = {2.0, 3.0, 4.0, 5.0};
    Vec v2 = {3.0, 4.0, 5.0, 6.0};

    Vec sum_of_vec_type = v0 + v1 + v2;

    for (int i=0; i != sum_of_vec_type.size(); ++i)
    {
        std::cout << sum_of_vec_type[i] << std::endl;
    }
}
