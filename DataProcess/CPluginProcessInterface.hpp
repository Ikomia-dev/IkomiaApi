#ifndef CPROCESSWIDGETINTERFACE_HPP
#define CPROCESSWIDGETINTERFACE_HPP

#include "Core/CProcessFactory.hpp"
#include "Core/CWidgetFactory.hpp"

/**
 * @ingroup groupDataProcess
 * @brief The CPluginProcessInterface class is an abstract class defining core structure of a plugin interface.
 * A plugin is a user-defined process task that can be integrated automatically into the system.
 * For such, it must respect this architecture based on the well known factory design pattern.
 * A plugin must provide and implement two factory classes: one for the process task and one for his widget.
 * It should also override this CPluginProcessInterface class and provides an implementation for the two factory getters.
 */
class CPluginProcessInterface
{
    public:

        /**
         * @brief Pure virtual method that gets the process task factory.
         * @return CProcessFactory based shared pointer.
         */
        virtual std::shared_ptr<CProcessFactory>        getProcessFactory() = 0;
        /**
         * @brief Pure virtual method that gets the process widget factory.
         * @return CWidgetFactory based shared pointer.
         */
        virtual std::shared_ptr<CWidgetFactory>         getWidgetFactory() = 0;
};

#define ProcessPlugin_iid  "ikomia.plugin.process"

Q_DECLARE_INTERFACE(CPluginProcessInterface, ProcessPlugin_iid)

#endif // CPROCESSWIDGETINTERFACE_HPP
