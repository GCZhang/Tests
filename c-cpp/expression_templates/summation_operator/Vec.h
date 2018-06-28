#ifndef USERVEC_H
#define USERVEC_H

#include <vector>
#include <initializer_list>
#include "VecSum.h"

class Vec : public VecExpression<Vec> {
    std::vector<double> elems;
    
    public:
    double operator[] (std::size_t i) const {return elems[i];}
    double& operator[] (std::size_t i)      {return elems[i];}
    std::size_t size() const { return elems.size(); }
    
    Vec(std::size_t n) : elems(n) {}

    // constructor using initializer list
    Vec (std::initializer_list<double>init) {
        for (auto i : init) elems.push_back(i);
    }

    // construtor using sumExpress
    template <typename T>
    Vec(VecExpression<T> const& vec) : elems(vec.size()){
        for (std::size_t i = 0; i != vec.size(); ++i){
            elems[i] = vec[i];
        }
    }
};


#endif
