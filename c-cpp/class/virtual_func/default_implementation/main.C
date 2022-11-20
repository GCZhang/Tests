#include "derived.h"
#include "base.h"
#include <vector>
#include <string>

int main(int args, char **argv)
{
    derived variable_derived(42);
    variable_derived.print();
}
