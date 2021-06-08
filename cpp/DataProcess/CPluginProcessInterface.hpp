// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef CPROCESSWIDGETINTERFACE_HPP
#define CPROCESSWIDGETINTERFACE_HPP

#include "Core/CTaskFactory.hpp"
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
         * @return CTaskFactory based shared pointer.
         */
        virtual std::shared_ptr<CTaskFactory>        getProcessFactory() = 0;
        /**
         * @brief Pure virtual method that gets the process widget factory.
         * @return CWidgetFactory based shared pointer.
         */
        virtual std::shared_ptr<CWidgetFactory>         getWidgetFactory() = 0;
};

#define ProcessPlugin_iid  "ikomia.plugin.process"

Q_DECLARE_INTERFACE(CPluginProcessInterface, ProcessPlugin_iid)

#endif // CPROCESSWIDGETINTERFACE_HPP
