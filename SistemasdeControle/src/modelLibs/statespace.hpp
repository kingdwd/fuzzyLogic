#ifdef testModel
    #include "../../../headers/modelLibs/statespace.h"
#else
    #include "SistemasdeControle/headers/modelLibs/statespace.h"
#endif

template <typename Type>
ModelHandler::StateSpace<Type>::StateSpace()
{
    this->continuous         = true;
    this->step               =  0.1;
    this->timeSimulation     =   10;
    this->nDiscretization    =    6;
    this->firstTimeKalmanObserver = 0;
}

template <typename Type>
ModelHandler::StateSpace<Type>::StateSpace(LinAlg::Matrix<Type> A, LinAlg::Matrix<Type> B,
                                 LinAlg::Matrix<Type> C, LinAlg::Matrix<Type> D)
{
    this->A                  =    A;
    this->B                  =    B;
    this->C                  =    C;
    this->D                  =    D;
    this->continuous         = true;
    this->step               =  0.1;
    this->timeSimulation     =   10;
    this->nDiscretization    =    6;
    this->firstTimeKalmanObserver = 0;

    this->initialState = LinAlg::Zeros<Type> (A.getNumberOfRows(),1);
    this->X = this->initialState;
}

template <typename Type>
ModelHandler::StateSpace<Type>::StateSpace(LinAlg::Matrix<Type> Ad, LinAlg::Matrix<Type> Bd,
                                 LinAlg::Matrix<Type> C , LinAlg::Matrix<Type> D ,
                                 Type step)
{
    this->Ad                 =    Ad;
    this->Bd                 =    Bd;
    this->C                  =    C;
    this->D                  =    D;
    this->continuous         = false;
    this->step               = step;
    this->timeSimulation     =   10;
    this->firstTimeKalmanObserver = 0;
    this->nDiscretization    =    6;

    this->initialState = LinAlg::Zeros<Type>(Ad.getNumberOfRows(),1);
    this->X = this->initialState;
}

template<typename Type> template<typename OtherType>
ModelHandler::StateSpace<Type>::StateSpace(const ModelHandler::StateSpace<OtherType>& otherStateSpaceFunction)
{
    *(this->A,this->B,this->C,this->D) = otherStateSpaceFunction.getContinuousParameters();
    *(this->Ad,this->Bd,this->C,this->D) = otherStateSpaceFunction.getDiscreteParameters();
    this->X  = otherStateSpaceFunction.getActualState();
    this->L  = otherStateSpaceFunction.getObserverParameters();
    this->P = otherStateSpaceFunction.getKalmanFilterParameters();
    this->firstTimeKalmanObserver = 0;
    this->continuous = otherStateSpaceFunction.isContinuous();
    this->step         = otherStateSpaceFunction.getSampleTime();
    this->initialState = otherStateSpaceFunction.getActualState();

    this->timeSimulation  = otherStateSpaceFunction.getTimeSimulation();
    this->nDiscretization = otherStateSpaceFunction.getnDiscretizationParameter();
}

template <typename Type>
ModelHandler::StateSpace<Type>& ModelHandler::StateSpace<Type>::operator= (const ModelHandler::StateSpace<Type>& otherStateSpaceFunction)
{
    *(this->A,this->B,this->C,this->D) = otherStateSpaceFunction.getContinuousParameters();
    *(this->Ad,this->Bd,this->C,this->D) = otherStateSpaceFunction.getDiscreteParameters();
    this->X  = otherStateSpaceFunction.getActualState();
    this->L  = otherStateSpaceFunction.getObserverParameters();
    this->P = otherStateSpaceFunction.getKalmanFilterParameters();
    this->firstTimeKalmanObserver = 0;
    this->continuous = otherStateSpaceFunction.isContinuous();
    this->step         = otherStateSpaceFunction.getSampleTime();
    this->initialState = otherStateSpaceFunction.getActualState();

    this->timeSimulation  = otherStateSpaceFunction.getTimeSimulation();
    this->nDiscretization = otherStateSpaceFunction.getnDiscretizationParameter();

    return *this;
}

template<typename Type> template<typename OtherStateSpaceFunctionType>
ModelHandler::StateSpace<Type>& ModelHandler::StateSpace<Type>::operator= (const ModelHandler::StateSpace<OtherStateSpaceFunctionType>& otherStateSpaceFunction)
{
    *(this->A,this->B,this->C,this->D) = otherStateSpaceFunction.getContinuousParameters();
    *(this->Ad,this->Bd,this->C,this->D) = otherStateSpaceFunction.getDiscreteParameters();
    this->X  = otherStateSpaceFunction.getActualState();
    this->L  = otherStateSpaceFunction.getObserverParameters();
    this->P = otherStateSpaceFunction.getKalmanFilterParameters();
    this->firstTimeKalmanObserver = 0;
    this->continuous = otherStateSpaceFunction.isContinuous();
    this->step         = otherStateSpaceFunction.getSampleTime();
    this->initialState = otherStateSpaceFunction.getActualState();

    this->timeSimulation  = otherStateSpaceFunction.getTimeSimulation();
    this->nDiscretization = otherStateSpaceFunction.getnDiscretizationParameter();

    return *this;
}

template <typename Type>
bool ModelHandler::StateSpace<Type>::isContinuous() const
{
    return this->continuous;
}

template <typename Type>
Type ModelHandler::StateSpace<Type>::getSampleTime() const
{
    return this->step;
}

template <typename Type>
LinAlg::Matrix< LinAlg::Matrix<Type>* >* ModelHandler::StateSpace<Type>::getContinuousParameters() const
{
    LinAlg::Matrix< LinAlg::Matrix<Type>* > *A = new LinAlg::Matrix< LinAlg::Matrix<Type>* >(1,4);
    (*A)(1,1) = new LinAlg::Matrix<Type>(this->A.getNumberOfRows(), this->A.getNumberOfColumns());
    (*A)(1,2) = new LinAlg::Matrix<Type>(this->B.getNumberOfRows(), this->B.getNumberOfColumns());
    (*A)(1,3) = new LinAlg::Matrix<Type>(this->C.getNumberOfRows(), this->C.getNumberOfColumns());
    (*A)(1,4) = new LinAlg::Matrix<Type>(this->D.getNumberOfRows(), this->D.getNumberOfColumns());
    (*((*A)(1,1))) = this->A;
    (*((*A)(1,2))) = this->B;
    (*((*A)(1,3))) = this->C;
    (*((*A)(1,4))) = this->D;

    return A;
}

template <typename Type>
LinAlg::Matrix< LinAlg::Matrix<Type>* >* ModelHandler::StateSpace<Type>::getDiscreteParameters() const
{
    LinAlg::Matrix< LinAlg::Matrix<Type>* > *A = new LinAlg::Matrix< LinAlg::Matrix<Type>* >(1,4);
    (*A)(1,1) = new LinAlg::Matrix<Type>(this->Ad.getNumberOfRows(), this->Ad.getNumberOfColumns());
    (*A)(1,2) = new LinAlg::Matrix<Type>(this->Bd.getNumberOfRows(), this->Bd.getNumberOfColumns());
    (*A)(1,3) = new LinAlg::Matrix<Type>(this->C.getNumberOfRows(), this->C.getNumberOfColumns());
    (*A)(1,4) = new LinAlg::Matrix<Type>(this->D.getNumberOfRows(), this->D.getNumberOfColumns());
    (*((*A)(1,1))) = this->Ad;
    (*((*A)(1,2))) = this->Bd;
    (*((*A)(1,3))) = this->C;
    (*((*A)(1,4))) = this->D;

    return A;
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::getA() const
{
    if(this->continuous)
        return this->A;
    else
        return this->Ad;
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::getB() const
{
    if(this->continuous)
        return this->B;
    else
        return this->Bd;
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::getC() const
{
    return this->C;
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::getD() const
{
    return this->D;
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::getActualState() const
{
    return X;
}

template <typename Type>
void ModelHandler::StateSpace<Type>::setA(LinAlg::Matrix<Type> A)
{
    if(this->continuous)
        this->A = A;
    else
        this->Ad = A;
}

template <typename Type>
void ModelHandler::StateSpace<Type>::setB(LinAlg::Matrix<Type> B)
{
    if(this->continuous)
        this->B = B;
    else
        this->Bd = B;
}

template <typename Type>
void ModelHandler::StateSpace<Type>::setC(LinAlg::Matrix<Type> C)
{
    this->C = C;
}

template <typename Type>
void ModelHandler::StateSpace<Type>::setD(LinAlg::Matrix<Type> D)
{
    this->D = D;
}

template <typename Type>
void ModelHandler::StateSpace<Type>::setSampleTime(Type step)
{
    this->step = step;
}

template <typename Type>
void ModelHandler::StateSpace<Type>::setContinuous(bool Continuous)
{
    this->continuous = Continuous;
}

template <typename Type>
void ModelHandler::StateSpace<Type>::setInitialState(LinAlg::Matrix<Type> X0)
{
    this->initialState = X0;
    this->X = X0;
}

template <typename Type>
void ModelHandler::StateSpace<Type>::setLinearModel(LinAlg::Matrix<Type> Input, LinAlg::Matrix<Type> Output)
{

}

template <typename Type>
void ModelHandler::StateSpace<Type>::setLinearVector(LinAlg::Matrix<Type> Input, LinAlg::Matrix<Type> Output)
{

}

template <typename Type>
void ModelHandler::StateSpace<Type>::setObserverParameters(LinAlg::Matrix<Type> L)
{
    this->L = L;
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::getContinuousObserverParametersByAckerman(LinAlg::Matrix<Type> polesToBePlaced)
{
    LinAlg::Matrix<Type> Qo;
    this->P = LinAlg::Zeros<Type>(A.getNumberOfRows(), A.getNumberOfColumns());
    for (unsigned i = 0; i < A.getNumberOfColumns(); ++i)
        Qo = Qo||(C*(A^i));

    for (unsigned i = 0; i <= A.getNumberOfColumns(); ++i)
        this->P += ((~A)^i)*polesToBePlaced(1,polesToBePlaced.getNumberOfColumns()-i);

    this->L = ~((LinAlg::Zeros<Type>(1,A.getNumberOfRows()-1)|1)*(((~Qo)^-1)*this->P));

    return this->L;
}

template <typename Type>
bool ModelHandler::StateSpace<Type>::isObservable() const
{
    LinAlg::Matrix<Type> Qo;
    if(this->continuous)
        for (unsigned i = 0; i < A.getNumberOfColumns(); ++i)
            Qo = Qo||(C*(A^i));
    else
        for (unsigned i = 0; i < Ad.getNumberOfColumns(); ++i)
            Qo = Qo||(C*(Ad^i));

//    std::cout << Qo;
    return (LinAlg::Determinant(Qo) != 0);
}

template <typename Type>
bool ModelHandler::StateSpace<Type>::isControlable() const
{
    LinAlg::Matrix<Type> Qc;
    if(this->continuous)
        for (unsigned i = 0; i < A.getNumberOfColumns(); ++i)
            Qc = Qc|((A^i)*B);
    else
        for (unsigned i = 0; i < Ad.getNumberOfColumns(); ++i)
            Qc = Qc|((Ad^i)*Bd);

//    std::cout << (B) << ((A)*B) << Qc;
    return (LinAlg::Determinant(Qc) != 0);
}

template <typename Type>
Type ModelHandler::StateSpace<Type>::sim(Type u)
{
    if(this->continuous)
        this->c2dConversion();

//    X = initialState;
    LinAlg::Matrix<Type> Xi1 = Ad*X+Bd*u;
    LinAlg::Matrix<Type> y  = C*X+D*u;
    X = Xi1;

    return y(1,1);
}

template <typename Type>
Type ModelHandler::StateSpace<Type>::sim(Type u, Type y)
{
    y = u;
    u = y;
    return 0;
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::sim(LinAlg::Matrix<Type> u)
{
    if(this->continuous)
        this->c2dConversion();

    LinAlg::Matrix<Type> y;
    for(unsigned i = 0; i < u.getNumberOfColumns(); ++i)
    {
        LinAlg::Matrix<Type> Xi1 = Ad*X+Bd*u.getColumn(i+1);
        y = y|(C*X + D*u.getColumn(i+1));
        X = Xi1;
    }

    return y;
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::sim(LinAlg::Matrix<Type> u, LinAlg::Matrix<Type> y)
{
    u = y;
    return LinAlg::Matrix<Type>(0.0);
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::sim(Type lmin, Type lmax, Type step)
{
    if(this->continuous)
        this->c2dConversion();

    LinAlg::Matrix<Type> y;
    X = initialState;
    unsigned cont = 1;
    for(Type i = lmin; i <= lmax; i+= step)
    {
        LinAlg::Matrix<Type> Xi1 = Ad*X+Bd*i;
        y = y|(C*X+D*i);
        X = Xi1;
        cont++;
    }

    return y;
}

template <typename Type>
std::string ModelHandler::StateSpace<Type>::print()
{
    std::string output;

    if(this->continuous == true){
        output += "The Continuous State Space Model is: \n\nA = \n";
        output << this->A; output += "\n\nB = \n";
        output << this->B; output += "\n\nC = \n";

    }else{
        output += "The Discrete State Space Model is: \n\nA = \n";
        output << this->Ad; output += "\n\nB = \n";
        output << this->Bd; output += "\n\nC = \n";
    }

    output << this->C; output += "\n\nD = \n";
    output << this->D; output += '\n';

    return output;
}

template <typename Type>
Type ModelHandler::StateSpace<Type>::factorial(unsigned n)
{
    Type retval = 1;

    for (int i = n; i > 1; --i)
        retval *= i;

    return retval;
}

template <typename Type>
void ModelHandler::StateSpace<Type>::c2dConversion()
{
//Pade
//    unsigned Nsize = nDiscretization, Dsize = nDiscretization;
//    LinAlg::Matrix<Type> Npq = LinAlg::Zeros<Type>(this->A.getNumberOfRows(), this->A.getNumberOfColumns());
//    LinAlg::Matrix<Type> Dpq = LinAlg::Zeros<Type>(this->A.getNumberOfRows(), this->A.getNumberOfColumns());
//
//    unsigned factor =  (unsigned)(LinAlg::max(LinAlg::abs(LinAlg::EigenValues(A))));
//
//    for(unsigned n = 0; n < Nsize; ++n)
//        Npq += ((factorial(Nsize + Dsize - n)*factorial(Nsize)/(factorial(Nsize + Dsize)*factorial(n)*(Nsize - n))))*((A*step/factor)^n);

//    for(unsigned n = 0; n < Dsize; ++n)
//        Dpq += ((factorial(Nsize + Dsize - n)*factorial(Dsize)/(factorial(Nsize + Dsize)*factorial(n)*(Dsize - n))))*((-A*step/factor)^n);

//    Ad = ((Dpq^-1)*Npq)^factor;

//Taylor
    this->Ad = LinAlg::Zeros<Type>(this->A.getNumberOfRows(), this->A.getNumberOfColumns());

    LinAlg::Matrix<Type> max, maxInd;
    *(max, maxInd) = LinAlg::max(LinAlg::abs(A));
    *(max, maxInd) = LinAlg::max(LinAlg::abs(max));
    unsigned factor =  (unsigned)(max(1,1));

    //taylor
    for(unsigned i = 0; i < nDiscretization; ++i){
        Ad += (1/factorial(i))*((A*this->step/factor)^i);
//        std::cout << A <<"\n"<< Ad;
    }
    Ad ^= factor;
//    std::cout << (Ad - (Ad^0)) << std::endl;
//    std::cout << (A^-1) << std::endl;
    Bd = (A^-1)*(Ad - (Ad^0))*B;
}

template <typename Type>
void ModelHandler::StateSpace<Type>::d2cConversion()
{
    LinAlg::Matrix<Type> I = LinAlg::Eye<Type> (Ad.getNumberOfRows());

    A = (Ad - I)/this->step;
    B = (((A^-1)*(Ad - I))^-1)*Bd;
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::ObserverLoop(LinAlg::Matrix<Type> U, LinAlg::Matrix<Type> Y)
{
    if(this->continuous)
        this->c2dConversion();
    if(this->L.getNumberOfRows() != this->A.getNumberOfRows())
       this->L = this->getContinuousObserverParametersByAckerman(LinAlg::Ones<Type>(1,this->A.getNumberOfRows()));
    X += this->step*(A*X + B*U + L*((Y - C*X)(1,1)));
    return X;
}

template <typename Type>
LinAlg::Matrix<Type> ModelHandler::StateSpace<Type>::KalmanFilterObserverLoop(LinAlg::Matrix<Type> U, LinAlg::Matrix<Type> Y)
{
    if(this->continuous)
        this->c2dConversion();
//    if(!firstTimeKalmanObserver)
//    P = LinAlg::Zeros<Type>(Ad.getNumberOfRows(),Ad.getNumberOfColumns());
    P = LinAlg::Eye<Type>(Ad.getNumberOfRows());
    LinAlg::Matrix<Type> Q = 10*LinAlg::Eye<Type>(Ad.getNumberOfRows());
    Type R = 1;

//    std::cout << Ad*P*(~Ad)<< std::endl
//              << Ad*P*(~C) << std::endl
//              << C*P*(~C) << std::endl
//              << ((R + C*P*(~C))^-1) << std::endl
//              << C*P*Ad <<  std::endl
//              << P << std::endl;

    for(unsigned i = 0; i < P.getNumberOfColumns(); ++i)
        P = Q + Ad*P*(~Ad) - Ad*P*(~C)*((R + C*P*(~C))^-1)*C*P*Ad;

//    std::cout << Ad*P*(~C) << std::endl
//              << C*P*(~C) << std::endl
//              << ((R + C*P*(~C))^-1) << std::endl;
    L = Ad*P*(~C)*((R + C*P*(~C))^-1);

//              std::cout << L <<  std::endl
//              << P << std::endl;

    X = ( Ad - L*C )*X + Bd*U(1,1) + L*Y(1,1);
    return X;
}

template<typename Type>
std::ostream& ModelHandler::operator<< (std::ostream& output, ModelHandler::StateSpace<Type> SS)
{
    output << SS.print();
    return output;
}

template<typename Type>
std::string& ModelHandler::operator<< (std::string& output, ModelHandler::StateSpace<Type> SS)
{
    output += SS.print();
    return output;
}

template<typename Type>
Type ModelHandler::sim(ModelHandler::StateSpace<Type> &SS, Type u)
{
    return SS.sim(u);
}

template<typename Type>
Type ModelHandler::sim(ModelHandler::StateSpace<Type> &SS, Type u, Type y)
{
    return SS.sim(u,y);
}

template<typename Type>
LinAlg::Matrix<Type> ModelHandler::sim(ModelHandler::StateSpace<Type> &SS, LinAlg::Matrix<Type> u)
{
    return SS.sim(u);
}

template<typename Type>
LinAlg::Matrix<Type> ModelHandler::sim(ModelHandler::StateSpace<Type> &SS, Type lmin, Type lmax, Type step)
{
    return SS.sim(lmin, lmax, step);
}

template<typename Type>
LinAlg::Matrix<Type> ModelHandler::sim(ModelHandler::StateSpace<Type> &SS, LinAlg::Matrix<Type> u, LinAlg::Matrix<Type> y)
{
    return SS.sim(u,y);
}
