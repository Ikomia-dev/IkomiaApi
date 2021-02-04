#ifndef CPROCESSFACTORYWRAP_H
#define CPROCESSFACTORYWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CProcessFactory.hpp"

class CProcessFactoryWrap: public CProcessFactory, public wrapper<CProcessFactory>
{
    public:

        virtual ProtocolTaskPtr create() override;
        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override;
};

#endif // CPROCESSFACTORYWRAP_H
