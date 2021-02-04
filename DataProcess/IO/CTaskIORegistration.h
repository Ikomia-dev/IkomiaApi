#ifndef CTASKIOREGISTRATION_H
#define CTASKIOREGISTRATION_H

#include "DataProcessGlobal.hpp"
#include "Protocol/CProtocolTaskIO.h"

class DATAPROCESSSHARED_EXPORT CTaskIORegistration
{
    public:

        CTaskIORegistration();

        const CProtocolTaskIOAbstractFactory&   getFactory() const;

        void                                    registerIO(const std::shared_ptr<CProtocolTaskIOFactory>& pFactory);

        void                                    reset();

    private:

        void                                    registerCore();

    private:

        CProtocolTaskIOAbstractFactory  m_factory;
};

#endif // CTASKIOREGISTRATION_H
