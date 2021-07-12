/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef CWORKFLOWWRAP_H
#define CWORKFLOWWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CWorkflow.h"
#include "Core/CIkomiaRegistry.h"


class CWorkflowWrap: public CWorkflow, public wrapper<CWorkflow>
{
    public:

        CWorkflowWrap();
        CWorkflowWrap(const std::string& name);
        CWorkflowWrap(const std::string &name, const std::shared_ptr<CIkomiaRegistry>& registryPtr);

        CWorkflowWrap(const CWorkflow &workflow);

        std::intptr_t               getRootID();
        std::vector<std::intptr_t>  getTaskIDs();
        WorkflowTaskPtr             getTask(std::intptr_t id);
        double                      getElapsedTimeTo(std::intptr_t id);
        std::vector<std::intptr_t>  getParents(std::intptr_t id);
        std::vector<std::intptr_t>  getChilds(std::intptr_t id);
        std::vector<size_t>         getInEdges(std::intptr_t id);
        std::vector<size_t>         getOutEdges(std::intptr_t id);
        boost::python::tuple        getEdgeInfo(size_t id);
        std::intptr_t               getEdgeSource(size_t id);
        std::intptr_t               getEdgeTarget(size_t id);

        std::intptr_t               addTaskWrap(const WorkflowTaskPtr &taskPtr);

        void                        connectWrap(const std::intptr_t& src, const std::intptr_t& target, int srcIndex, int targetIndex);

        void                        deleteTaskWrap(std::intptr_t id);
        void                        deleteEdgeWrap(size_t id);

        void                        run() override;
        void                        default_run();

        void                        clearWrap();

        void                        loadWrap(const std::string& path);

    private:

        std::pair<bool, WorkflowEdge>   getEdgeDescriptor(size_t index) const;

        void                            removeEdgeIndex(const WorkflowEdge &edge);

    private:

        size_t                          m_edgeIndex = 0;
        std::map<WorkflowEdge, size_t>  m_edgeDescToIndex;
};

#endif // CWORKFLOWWRAP_H
