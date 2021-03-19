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

#ifndef CPROTOCOLTASKWRAP_HPP
#define CPROTOCOLTASKWRAP_HPP

#include "PyCoreGlobal.h"
#include "Protocol/CProtocolTask.h"

//----------------------------------------------//
//------------  CProtocolTaskWrap  -------------//
//- Wrapping class to handle virtual functions -//
//----------------------------------------------//
class CProtocolTaskWrap : public CProtocolTask, public wrapper<CProtocolTask>
{
    public:

        CProtocolTaskWrap();
        CProtocolTaskWrap(const std::string& name);
        CProtocolTaskWrap(const CProtocolTask& task);

        virtual ~CProtocolTaskWrap() = default;

        virtual void    setInputDataType(const IODataType& dataType, size_t index = 0) override;
        void            default_setInputDataType(const IODataType& dataType, size_t index = 0);

        virtual void    setInput(const ProtocolTaskIOPtr& pInput, size_t index = 0, bool bNewSequence=false) override;
        void            default_setInput(const ProtocolTaskIOPtr& pInput, size_t index = 0, bool bNewSequence=false);

        virtual void    setInputs(const InputOutputVect& inputs, bool bNewSequence) override;
        void            default_setInputs(const InputOutputVect& inputs, bool bNewSequence);

        virtual void    setOutputDataType(const IODataType& dataType, size_t index = 0) override;
        void            default_setOutputDataType(const IODataType& dataType, size_t index = 0);

        virtual void    setOutput(const ProtocolTaskIOPtr& pOutput, size_t index = 0) override;
        void            default_setOutput(const ProtocolTaskIOPtr& pOutput, size_t index = 0);

        virtual void    setOutputs(const InputOutputVect &outputs) override;
        void            default_setOutputs(const InputOutputVect &outputs);

        virtual void    setParam(const ProtocolTaskParamPtr& pParam) override;
        void            default_setParam(const ProtocolTaskParamPtr& pParam);

        virtual void    setActive(bool bActive) override;
        void            default_setActive(bool bActive);

        virtual size_t  getProgressSteps() override;
        size_t          default_getProgressSteps();

        virtual size_t  getProgressSteps(size_t unitEltCount) override;
        size_t          default_getProgressSteps(size_t unitEltCount);

        InputOutputVect getInputs(const std::vector<IODataType>& types) const;
        InputOutputVect getOutputs(const std::vector<IODataType>& types) const;

        virtual bool    isGraphicsChangedListening() const override;
        bool            default_isGraphicsChangedListening() const;

        virtual void    addInput(const ProtocolTaskIOPtr& pInput) override;
        void            default_addInput(const ProtocolTaskIOPtr& pInput);

        virtual void    addOutput(const ProtocolTaskIOPtr& pOutput) override;
        void            default_addOutput(const ProtocolTaskIOPtr& pOutput);

        virtual void    run() override;
        void            default_run();

        virtual void    stop() override;
        void            default_stop();

        virtual void    executeActions(int flags) override;
        void            default_executeActions(int flags);

        virtual void    updateStaticOutputs() override;
        void            default_updateStaticOutputs();

        virtual void    beginTaskRun() override;
        void            default_beginTaskRun();

        virtual void    endTaskRun() override;
        void            default_endTaskRun();

        virtual void    parametersModified() override;
        void            default_parametersModified();

        virtual void    graphicsChanged() override;
        void            default_graphicsChanged();

        virtual void    globalInputChanged(bool bNewSequence) override;
        void            default_globalInputChanged(bool bNewSequence);

        //Signal wrapped
        void            emitAddSubProgressSteps(int count);
        void            emitStepProgress();
        void            emitGraphicsContextChanged();
        void            emitOutputChanged();
};

#endif // CPROTOCOLTASKWRAP_HPP
