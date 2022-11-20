#pragma once

#include "base.h"

class derived : public base
{
private:
    int _i;

public:
    derived(int i);
    void print();
    virtual void printVariable() const override;
};

derived::derived(int i) : base(i)
{
}

void derived::print()
{
    _i = 1;
    set_i(43);
    printVariable();
    std::cout << "_i in derived class is: " << _i << std::endl;
    base::printVariable();
}

void derived::printVariable() const
{
    std::cout << "_i in derived class is: " << _i << std::endl;
}
