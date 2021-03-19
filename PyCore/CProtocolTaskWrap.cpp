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

#include "CProtocolTaskWrap.h"
#include "PyCoreTools.hpp"

//----------------------------------------------//
//------------  CProtocolTaskWrap  -------------//
//----------------------------------------------//
CProtocolTaskWrap::CProtocolTaskWrap() : CProtocolTask()
{
}

CProtocolTaskWrap::CProtocolTaskWrap(const std::string &name) : CProtocolTask(name)
{
}

CProtocolTaskWrap::CProtocolTaskWrap(const CProtocolTask &task) : CProtocolTask(task)
{
}

void CProtocolTaskWrap::setInputDataType(const IODataType &dataType, size_t index)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setOver = this->get_override("setInputDataType"))
            setOver(dataType, index);
        else
            CProtocolTask::setInputDataType(dataType, index);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_setInputDataType(const IODataType &dataType, size_t index)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::setInputDataType(dataType, index);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::setInput(const ProtocolTaskIOPtr &pInput, size_t index, bool bNewSequence)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setInputOver = this->get_override("setInput"))
            setInputOver(pInput, index);
        else
            CProtocolTask::setInput(pInput, index, bNewSequence);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_setInput(const ProtocolTaskIOPtr &pInput, size_t index, bool bNewSequence)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::setInput(pInput, index, bNewSequence);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::setInputs(const InputOutputVect &inputs, bool bNewSequence)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setInputsOver = this->get_override("setInputs"))
            setInputsOver(inputs, bNewSequence);
        else
            CProtocolTask::setInputs(inputs, bNewSequence);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_setInputs(const InputOutputVect &inputs, bool bNewSequence)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::setInputs(inputs, bNewSequence);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::setOutputDataType(const IODataType &dataType, size_t index)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setOutputDataOver = this->get_override("setOutputDataType"))
            setOutputDataOver(dataType, index);
        else
            CProtocolTask::setOutputDataType(dataType, index);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_setOutputDataType(const IODataType &dataType, size_t index)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::setOutputDataType(dataType, index);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::setOutput(const ProtocolTaskIOPtr &pOutput, size_t index)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setOutputOver = this->get_override("setOutput"))
            setOutputOver(pOutput, index);
        else
            CProtocolTask::setOutput(pOutput, index);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_setOutput(const ProtocolTaskIOPtr &pOutput, size_t index)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::setOutput(pOutput, index);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::setOutputs(const InputOutputVect &outputs)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setOutputsOver = this->get_override("setOutputs"))
            setOutputsOver(outputs);
        else
            CProtocolTask::setOutputs(outputs);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_setOutputs(const InputOutputVect &outputs)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::setOutputs(outputs);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::setParam(const ProtocolTaskParamPtr &pParam)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setParamOver = this->get_override("setParam"))
            setParamOver(pParam);
        else
            CProtocolTask::setParam(pParam);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_setParam(const ProtocolTaskParamPtr &pParam)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::setParam(pParam);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::setActive(bool bActive)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setActiveOver = this->get_override("setActive"))
            setActiveOver(bActive);
        else
            CProtocolTask::setActive(bActive);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_setActive(bool bActive)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::setActive(bActive);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CProtocolTaskWrap::getProgressSteps()
{
    CPyEnsureGIL gil;
    try
    {
        if(override getProgressStepsOver = this->get_override("getProgressSteps"))
            return getProgressStepsOver();

        return CProtocolTask::getProgressSteps();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CProtocolTaskWrap::default_getProgressSteps()
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProtocolTask::getProgressSteps();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CProtocolTaskWrap::getProgressSteps(size_t unitEltCount)
{
    CPyEnsureGIL gil;
    try
    {
        if(override getProgressStepsOver = this->get_override("getProgressSteps"))
            return getProgressStepsOver(unitEltCount);

        return CProtocolTask::getProgressSteps(unitEltCount);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CProtocolTaskWrap::default_getProgressSteps(size_t unitEltCount)
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProtocolTask::getProgressSteps(unitEltCount);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

InputOutputVect CProtocolTaskWrap::getInputs(const std::vector<IODataType> &types) const
{
    std::set<IODataType> inTypes(types.begin(), types.end());
    return CProtocolTask::getInputs(inTypes);
}

InputOutputVect CProtocolTaskWrap::getOutputs(const std::vector<IODataType> &types) const
{
    std::set<IODataType> outTypes(types.begin(), types.end());
    return CProtocolTask::getOutputs(outTypes);
}

bool CProtocolTaskWrap::isGraphicsChangedListening() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isGraphicsChangedOver = this->get_override("isGraphicsChangedListening"))
            return isGraphicsChangedOver();

        return CProtocolTask::isGraphicsChangedListening();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CProtocolTaskWrap::default_isGraphicsChangedListening() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProtocolTask::isGraphicsChangedListening();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::addInput(const ProtocolTaskIOPtr &pInput)
{
    CPyEnsureGIL gil;
    try
    {
        if(override addInputOver = this->get_override("addInput"))
            addInputOver(pInput);
        else
            CProtocolTask::addInput(pInput);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_addInput(const ProtocolTaskIOPtr &pInput)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::addInput(pInput);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::addOutput(const ProtocolTaskIOPtr &pOutput)
{
    CPyEnsureGIL gil;
    try
    {
        if(override addOutputOver = this->get_override("addOutput"))
            addOutputOver(pOutput);
        else
            CProtocolTask::addOutput(pOutput);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_addOutput(const ProtocolTaskIOPtr &pOutput)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::addOutput(pOutput);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::run()
{
    CPyEnsureGIL gil;
    try
    {
        if(override runOver = this->get_override("run"))
            runOver();
        else
            CProtocolTask::run();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_run()
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::run();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::stop()
{
    CPyEnsureGIL gil;
    try
    {
        if(override stopOver = this->get_override("stop"))
            stopOver();
        else
            CProtocolTask::stop();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_stop()
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::stop();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::executeActions(int flags)
{
    CPyEnsureGIL gil;
    try
    {
        if(override executeActionsOver = this->get_override("executeActions"))
            executeActionsOver(flags);
        else
            CProtocolTask::executeActions(flags);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_executeActions(int flags)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::executeActions(flags);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::updateStaticOutputs()
{
    CPyEnsureGIL gil;
    try
    {
        if(override updateStaticOutputsOver = this->get_override("updateStaticOutputs"))
            updateStaticOutputsOver();
        else
            CProtocolTask::updateStaticOutputs();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_updateStaticOutputs()
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::updateStaticOutputs();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::beginTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        if(override beginTaskRunOver = this->get_override("beginTaskRun"))
            beginTaskRunOver();
        else
            CProtocolTask::beginTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_beginTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::beginTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::endTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        if(override endTaskRunOver = this->get_override("endTaskRun"))
            endTaskRunOver();
        else
            CProtocolTask::endTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_endTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::endTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::parametersModified()
{
    CPyEnsureGIL gil;
    try
    {
        if(override parametersModifiedOver = this->get_override("parametersModified"))
            parametersModifiedOver();
        else
            CProtocolTask::parametersModified();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_parametersModified()
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::parametersModified();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::graphicsChanged()
{
    CPyEnsureGIL gil;
    try
    {
        if(override graphicsChangedOver = this->get_override("graphicsChanged"))
            graphicsChangedOver();
        else
            CProtocolTask::graphicsChanged();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_graphicsChanged()
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::graphicsChanged();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::globalInputChanged(bool bNewSequence)
{
    CPyEnsureGIL gil;
    try
    {
        if(override globalInputChangedOver = this->get_override("globalInputChanged"))
            globalInputChangedOver(bNewSequence);
        else
            CProtocolTask::globalInputChanged(bNewSequence);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::default_globalInputChanged(bool bNewSequence)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTask::globalInputChanged(bNewSequence);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWrap::emitAddSubProgressSteps(int count)
{
    m_signalHandler->doAddSubTotalSteps(count);
}

void CProtocolTaskWrap::emitGraphicsContextChanged()
{
    emit m_signalHandler->doGraphicsContextChanged();
}

void CProtocolTaskWrap::emitStepProgress()
{
    emit m_signalHandler->doProgress();
}

void CProtocolTaskWrap::emitOutputChanged()
{
    emit m_signalHandler->doOutputChanged();
}

