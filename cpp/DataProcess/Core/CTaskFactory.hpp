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

#ifndef CTASKFACTORY_H
#define CTASKFACTORY_H

#include "DesignPattern/CAbstractFactory.hpp"
#include "Core/CTaskInfo.h"
#include "Workflow/CWorkflowTaskParam.h"
#include "Workflow/CWorkflowTask.h"

//------------------------//
//----- Task factory -----//
//------------------------//
/**
 * @ingroup groupDataProcess
 * @brief The CTaskFactory class is an abstract class defining the core structure of the process factory.
 * The system extensibility for the process library is based on the well known factory design pattern.
 * Each process task must implement a factory class derived from this class.
 * Then the system is able to instantiate dynamically a process object (even user-defined).
 */
class CTaskFactory
{
    public:

        virtual ~CTaskFactory() = default;

        /**
         * @brief Gets the metadata structure associated with the process task.
         * @return CTaskInfo reference.
         */
        CTaskInfo&              getInfo(){ return m_info; }
        /**
         * @brief Gets the metadata structure associated with the process task.
         * @return CTaskInfo object.
         */
        CTaskInfo               getInfo() const { return m_info; }

        /**
         * @brief Sets the metadata structure associated with the process task.
         * @param info: see CTaskInfo for more details.
         */
        void                    setInfo(const CTaskInfo& info){ m_info = info; }

        /**
         * @brief Pure virtual method to create new process task instance with the given parameters.
         * @param pParam: CWorkflowTaskParam based shared pointer.
         * @return CWorkflowTask based shared pointer.
         */
        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) = 0;
        /**
         * @brief Pure virtual method to create new process task instance with default parameters.
         * @return CWorkflowTask based shared pointer.
         */
        virtual WorkflowTaskPtr create() = 0;

    protected:

        CTaskInfo                m_info;
};

using TaskFactoryPtr = std::shared_ptr<CTaskFactory>;
using TaskFactories = std::vector<TaskFactoryPtr>;

//----- Task abstract factory -----//
class CTaskAbstractFactory: public CAbstractFactory<std::string, WorkflowTaskPtr, const WorkflowTaskParamPtr>
{
    public:

        TaskFactories&  getList()
        {
            return m_factories;
        }

        TaskFactoryPtr  getFactory(const std::string& name) const
        {
            auto it = std::find_if(m_factories.begin(),
                                   m_factories.end(),
                                   [&name](const TaskFactoryPtr& factoryPtr){ return factoryPtr->getInfo().getName() == name;});
            return *it;
        }

        void            remove(const std::string& name)
        {
            m_factories.erase(std::remove_if(m_factories.begin(),
                                             m_factories.end(),
                                             [name](const TaskFactoryPtr& factoryPtr){return factoryPtr->getInfo().getName() == name;}),
                              m_factories.end());
        }

        void            clear() override
        {
            CAbstractFactory<std::string, WorkflowTaskPtr, const WorkflowTaskParamPtr>::clear();
            m_factories.clear();
        }

    private:

        TaskFactories m_factories;
};

#endif // CTASKFACTORY_H
