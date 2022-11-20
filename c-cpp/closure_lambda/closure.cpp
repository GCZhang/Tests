#include <iostream>
#include <functional>

std::function<void(void)> closureWrapper1()
{
    int x = 10;
    // if we write "x += 1; ..." here, then the compiler would complain "error: assignment of read-only variable 'x'"
    return [x]()
    { std::cout << "Value in the closure: " << x << std::endl; };
}

std::function<void(void)> closureWrapper2()
{
    int x = 10;
    return [&x]()
    { x += 1; std::cout << "Value in the closure: " << x << std::endl; };
}

int main(int args, char *argv[])
{
    int x = 10;
    auto func0 = [&x]()
    { x += 1; std::cout << "Value in the closure: " << x << std::endl; };

    std::function<void(void)> func1 = closureWrapper1();

    std::function<void(void)> func2 = closureWrapper2();

    func0();
    func0();
    func0();
    std::cout << "-------------------------" << std::endl;

    func1();
    func1();
    func1();
    std::cout << "-------------------------" << std::endl;

    func2();
    func2();
    func2();
    std::cout << "-------------------------" << std::endl;

    return 0;
}
