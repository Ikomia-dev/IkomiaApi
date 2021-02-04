#ifndef CWIDGETFACTORYWRAP_H
#define CWIDGETFACTORYWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CWidgetFactory.hpp"

class CWidgetFactoryWrap : public CWidgetFactory, public wrapper<CWidgetFactory>
{
    public:

        virtual ProtocolTaskWidgetPtr  create(const ProtocolTaskParamPtr pParam);
};

#endif // CWIDGETFACTORYWRAP_H
