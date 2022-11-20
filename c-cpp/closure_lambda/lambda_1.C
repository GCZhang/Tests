#include <iostream>

int main(int args, char *argv[])
{
    int i = 0;

    // caputre i by reference.
    [&i]()
    { std::cout << "hello world " << ++i << " !" << std::endl; }(); // the last '()' calls the lambda function;

    // caputre i by reference.
    auto f = [&i]()
    { std::cout << "hello world " << ++i << " !" << std::endl; };

    f();
}
