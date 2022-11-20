#pragma once
#include <iostream>

class base
{
private:
    /* data */
    int _i;

public:
    base(int i);
    virtual void printVariable() const;
    void set_i(int i) { _i = i; }
};

base::base(int i) : _i(i)
{
}

void base::printVariable() const
{
    std::cout << "_i in base class is: " << _i << std::endl;
}
