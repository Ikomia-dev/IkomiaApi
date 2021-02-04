#ifndef CGMICTASKPARAM_HPP
#define CGMICTASKPARAM_HPP

#include "Protocol/CProtocolTaskParam.h"

class CGmicTaskParam: public CProtocolTaskParam
{
    public:

        CGmicTaskParam() : CProtocolTaskParam(){}

        virtual std::string getParamString() const = 0;
};

#endif // CGMICTASKPARAM_HPP
