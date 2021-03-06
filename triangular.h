#ifndef TRIANGULAR_H
#define TRIANGULAR_H
#include "membershipfunction.h"

namespace advancedModelHandler {
    template <typename Type>
    class triangular : public MembershipFunction<Type>
    {
        Type leftLimit, rigthLimit, peak;
    public:
        triangular(Type leftLimit, Type peak, Type rigthLimit);
        Type         sim(Type x);
        LinAlg::Matrix<Type> sim(LinAlg::Matrix<Type> x);
        Type getAverage(){ return this->peak; }
    };
}

#include "triangular.hpp"
#endif // TRIANGULAR_H
