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

#ifndef CWIDGETFACTORY_HPP
#define CWIDGETFACTORY_HPP

#include "DesignPattern/CAbstractFactory.hpp"
#include "DesignPattern/CFactoryRegister.hpp"
#include <QGridLayout>
#include "Protocol/CProtocolTaskWidget.h"

/**
 * @ingroup groupDataProcess
 * @brief The CWidgetFactory class is an abstract class defining the core structure of the process widget factory.
 * The system extensibility for the process library is based on the well known factory design pattern.
 * Each process task must implement a widget factory class derived from this class.
 * Then the system is able to instantiate dynamically a process widget object (even user-defined).
 */
class CWidgetFactory
{
    public:

        virtual ~CWidgetFactory() = default;

        /**
         * @brief Gets the associated process task name.
         * @return Process name.
         */
        std::string                     getName() const
        {
            return m_name;
        }

        /**
         * @brief Sets the associated process task name.
         * @param name: new process name.
         */
        void                            setName(const std::string& name)
        {
            m_name = name;
        }

        //Perfect design should force us to use std::unique_ptr here,
        //to translate ownership of raw pointer to Qt when needed.
        //However, we also need Python binding with Boost.Python that do not handle
        //std::unique_ptr (no move semantic). So we have to use std::shared_ptr and
        //ensure lifetime of shared pointers while Qt need raw pointers
        /**
         * @brief Pure virtual method to create new process widget instance with the given parameters.
         * @param pParam: CProtocolTaskParam based shared pointer.
         * @return CProtocolTaskWidget based shared pointer.
         */
        virtual ProtocolTaskWidgetPtr   create(const ProtocolTaskParamPtr pParam) = 0;

    protected:

        std::string m_name = "";
};

using WidgetFactoryPtr = std::shared_ptr<CWidgetFactory>;
using WidgetFactories = std::vector<WidgetFactoryPtr>;

//Process abstract factory
class CWidgetAbstractFactory: public CAbstractFactory<std::string, ProtocolTaskWidgetPtr, const ProtocolTaskParamPtr>
{
    public:

        WidgetFactories&    getList()
        {
            return m_factories;
        }

        void                remove(const std::string& name)
        {
            m_factories.erase(std::remove_if(m_factories.begin(),
                                             m_factories.end(),
                                             [name](const WidgetFactoryPtr& factoryPtr){return factoryPtr->getName() == name;}),
                              m_factories.end());
        }

        void                clear() override
        {
            CAbstractFactory<std::string, ProtocolTaskWidgetPtr, const ProtocolTaskParamPtr>::clear();
            m_factories.clear();
        }

    private:

        WidgetFactories m_factories;
};

#endif // CWIDGETFACTORY_HPP
