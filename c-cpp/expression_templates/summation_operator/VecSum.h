#ifndef VECSUM_H
#define VECSUM_H
#include "VecExpression.h"
#include <assert.h>
template <typename E1, typename E2> class VecSum;
template <typename E1, typename E2>
class VecSum : public VecExpression<VecSum<E1, E2> >{
    E1 const& _u;
    E2 const& _v;

public:
    VecSum(E1 const& u, E2 const& v): _u(u), _v(v){
        assert(u.size() == v.size());
    }
    
    double operator[](std::size_t i) const {return _u[i] + _v[i];}

    std::size_t size()                const {return _v.size();}
};

template <typename E1, typename E2>
VecSum<E1, E2> operator+ (E1 const& u, E2 const& v){
    return VecSum<E1, E2>(u, v);
}

#endif
