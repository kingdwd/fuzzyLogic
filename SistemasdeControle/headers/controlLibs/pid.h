#ifndef __PID_H_INCLUDED
#define __PID_H_INCLUDED

#include <cmath>

namespace ControlHandler{
    template <typename Type>
    class PID
    {
    private:
        bool checkUpLim, checkLowLim;
        Type Error, pastError, integralError, derivativeError, Step, kp, ki, kd, upperLimit, lowerLimit, PIDout;

        void errorLimitation();

        void intError();
        void difError();

    public:
        PID();
        PID(const LinAlg::Matrix<Type> &PIDsParameters);

        Type getSampleTime() const {return this->Step;}
        Type getErrorValue() const {return this->Error;}
        Type getDerivativeErrorValue() const {return this->derivativeError;}
        Type getIntegralErrorValue() const {return this->integralError;}
        LinAlg::Matrix<Type> getLimits();
        LinAlg::Matrix<Type> getParams();

        void setSampleTime(Type Time);
        void setLimits(Type upperLimit, Type lowerLimit);
        void setParams(Type kp, Type ki, Type kd);
        void setParams(const LinAlg::Matrix<Type> &PIDsParameters);

        Type OutputControl(Type Reference, Type SignalInput);
    };
}

#ifdef testControl
    #include "../../../src/controlLibs/pid.hpp"
#else
    #include "SistemasdeControle/src/controlLibs/pid.hpp"
#endif

#endif // PID_H
