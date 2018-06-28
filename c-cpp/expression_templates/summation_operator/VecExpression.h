#ifndef VECEXPRESSION_H
#define VECEXPRESSION_H
#include <vector>


template<typename T>
class VecExpression {
    public:
        double operator[](std::size_t i) const {return static_cast<T const&> (*this)[i];}
        std::size_t size()          const {return static_cast<T const&> (*this).size();}
};

#endif
