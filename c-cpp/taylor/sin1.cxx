#include<iostream>
#include<cmath>
using std::cout;
using std::cin;
// calculate sin(1) using taylor series
int main(int args, char** argv) {
    double term, x, f;
    int max_iter;

    x = 1.0;
    f  = 0.0;
    term = x;
    
    cout << "Input max iterations:";
    cin >> max_iter;

    for (int i=1; i!=max_iter; ++i) {
        f = f + term;
        term *= (-x * x) / ((2*i) * (2*i + 1));
    }
    cout << "Final value = " << f << std::endl;
    cout << "Reference   = " << sin(x) << std::endl;

    return 0;
}

