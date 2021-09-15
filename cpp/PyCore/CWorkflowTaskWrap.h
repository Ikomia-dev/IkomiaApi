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

#ifndef CWORKFLOWTASKWRAP_H
#define CWORKFLOWTASKWRAP_H

#include "PyCoreGlobal.h"
#include "Workflow/CWorkflowTask.h"

//----------------------------------------------//
//------------  CWorkflowTaskWrap  -------------//
//- Wrapping class to handle virtual functions -//
//----------------------------------------------//
class CWorkflowTaskWrap : public CWorkflowTask, public wrapper<CWorkflowTask>
{
    public:

        CWorkflowTaskWrap();
        CWorkflowTaskWrap(const std::string& name);
        CWorkflowTaskWrap(const CWorkflowTask& task);

        ~CWorkflowTaskWrap() = default;

        void    setInputDataType(const IODataType& dataType, size_t index = 0) override;
        void    default_setInputDataType(const IODataType& dataType, size_t index = 0);

        void    setInput(const WorkflowTaskIOPtr& pInput, size_t index = 0, bool bNewSequence=false) override;
        void    default_setInput(const WorkflowTaskIOPtr& pInput, size_t index = 0, bool bNewSequence=false);

        void    setInputs(const InputOutputVect& inputs, bool bNewSequence) override;
        void    default_setInputs(const InputOutputVect& inputs, bool bNewSequence);

        void    setOutputDataType(const IODataType& dataType, size_t index = 0) override;
        void    default_setOutputDataType(const IODataType& dataType, size_t index = 0);

        void    setOutput(const WorkflowTaskIOPtr& pOutput, size_t index = 0) override;
        void    default_setOutput(const WorkflowTaskIOPtr& pOutput, size_t index = 0);

        void    setOutputs(const InputOutputVect &outputs) override;
        void    default_setOutputs(const InputOutputVect &outputs);

        void    setActive(bool bActive) override;
        void    default_setActive(bool bActive);

        size_t  getProgressSteps() override;
        size_t  default_getProgressSteps();

        size_t  getProgressSteps(size_t unitEltCount) override;
        size_t  default_getProgressSteps(size_t unitEltCount);

        bool    isGraphicsChangedListening() const override;
        bool    default_isGraphicsChangedListening() const;

        void    addInput(const WorkflowTaskIOPtr& pInput) override;
        void    default_addInput(const WorkflowTaskIOPtr& pInput);

        void    addOutput(const WorkflowTaskIOPtr& pOutput) override;
        void    default_addOutput(const WorkflowTaskIOPtr& pOutput);

        void    run() override;
        void    default_run();

        void    stop() override;
        void    default_stop();

        void    executeActions(int flags) override;
        void    default_executeActions(int flags);

        void    updateStaticOutputs() override;
        void    default_updateStaticOutputs();

        void    beginTaskRun() override;
        void    default_beginTaskRun();

        void    endTaskRun() override;
        void    default_endTaskRun();

        void    parametersModified() override;
        void    default_parametersModified();

        void    graphicsChanged() override;
        void    default_graphicsChanged();

        void    globalInputChanged(bool bNewSequence) override;
        void    default_globalInputChanged(bool bNewSequence);

        void    workflowStarted() override;
        void    default_workflowStarted();

        void    workflowFinished() override;
        void    default_workflowFinished();

        //Signal wrapped
        void            emitAddSubProgressSteps(int count);
        void            emitStepProgress();
        void            emitGraphicsContextChanged();
        void            emitOutputChanged();
};

#endif // CWORKFLOWTASKWRAP_H
