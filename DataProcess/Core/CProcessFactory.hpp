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

#ifndef CPROCESSFACTORY_H
#define CPROCESSFACTORY_H

#include "DesignPattern/CAbstractFactory.hpp"
#include "Core/CProcessInfo.h"
#include "Protocol/CProtocolTaskParam.h"
#include "Protocol/CProtocolTask.h"

//---------------------------//
//----- Process factory -----//
//---------------------------//
/**
 * @ingroup groupDataProcess
 * @brief The CProcessFactory class is an abstract class defining the core structure of the process factory.
 * The system extensibility for the process library is based on the well known factory design pattern.
 * Each process task must implement a factory class derived from this class.
 * Then the system is able to instantiate dynamically a process object (even user-defined).
 */
class CProcessFactory
{
    public:

        virtual ~CProcessFactory() = default;

        /**
         * @brief Gets the metadata structure associated with the process task.
         * @return CProcessInfo reference.
         */
        CProcessInfo&               getInfo(){ return m_info; }
        /**
         * @brief Gets the metadata structure associated with the process task.
         * @return CProcessInfo object.
         */
        CProcessInfo                getInfo() const { return m_info; }

        /**
         * @brief Sets the metadata structure associated with the process task.
         * @param info: see CProcessInfo for more details.
         */
        void                        setInfo(const CProcessInfo& info){ m_info = info; }

        /**
         * @brief Pure virtual method to create new process task instance with the given parameters.
         * @param pParam: CProtocolTaskParam based shared pointer.
         * @return CProtocolTask based shared pointer.
         */
        virtual ProtocolTaskPtr     create(const ProtocolTaskParamPtr& pParam) = 0;
        /**
         * @brief Pure virtual method to create new process task instance with default parameters.
         * @return CProtocolTask based shared pointer.
         */
        virtual ProtocolTaskPtr     create() = 0;

    protected:

        CProcessInfo                m_info;
};

using ProcessFactoryPtr = std::shared_ptr<CProcessFactory>;
using ProcessFactories = std::vector<ProcessFactoryPtr>;

//----- Process abstract factory -----//
class CProcessAbstractFactory: public CAbstractFactory<std::string, ProtocolTaskPtr, const ProtocolTaskParamPtr>
{
    public:

        ProcessFactories&   getList()
        {
            return m_factories;
        }

        ProcessFactoryPtr   getFactory(const std::string& name)
        {
            auto it = std::find_if(m_factories.begin(),
                                   m_factories.end(),
                                   [&name](const ProcessFactoryPtr& factoryPtr){ return factoryPtr->getInfo().getName() == name;});
            return *it;
        }

        void                remove(const std::string& name)
        {
            m_factories.erase(std::remove_if(m_factories.begin(),
                                             m_factories.end(),
                                             [name](const ProcessFactoryPtr& factoryPtr){return factoryPtr->getInfo().getName() == name;}),
                              m_factories.end());
        }

    private:

        ProcessFactories m_factories;
};

#endif // CPROCESSFACTORY_H
