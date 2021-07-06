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

        std::vector<std::intptr_t>  getTaskIDs();
        WorkflowTaskPtr             getTask(std::intptr_t id);

        void    run() override;
        void    default_run();
};

#endif // CWORKFLOWWRAP_H
