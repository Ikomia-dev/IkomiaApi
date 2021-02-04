#ifndef CPLUGININTERFACEWRAP_H
#define CPLUGININTERFACEWRAP_H

#include "PyDataProcessGlobal.h"
#include "CPluginProcessInterface.hpp"

class CPluginProcessInterfaceWrap : public CPluginProcessInterface, public wrapper<CPluginProcessInterface>
{
    public:

        std::shared_ptr<CProcessFactory>    getProcessFactory();
        std::shared_ptr<CWidgetFactory>     getWidgetFactory();
};

#endif // CPLUGININTERFACEWRAP_H
