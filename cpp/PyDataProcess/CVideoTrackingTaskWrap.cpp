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

#include "CVideoTrackingTaskWrap.h"

//----------------------------------------------//
//-----------  CVideoTrackingTaskWrap  ------------//
//----------------------------------------------//
CVideoTrackingTaskWrap::CVideoTrackingTaskWrap() : CVideoTrackingTask()
{
}

CVideoTrackingTaskWrap::CVideoTrackingTaskWrap(const std::string &name) : CVideoTrackingTask(name)
{
}

CVideoTrackingTaskWrap::CVideoTrackingTaskWrap(const CVideoTrackingTask &process) : CVideoTrackingTask(process)
{
}

size_t CVideoTrackingTaskWrap::getProgressSteps()
{
    CPyEnsureGIL gil;
    try
    {
        if(override getProgressStepsOver = this->get_override("getProgressSteps"))
            return getProgressStepsOver();

        return CVideoTrackingTask::getProgressSteps();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CVideoTrackingTaskWrap::default_getProgressSteps()
{
    CPyEnsureGIL gil;
    try
    {
        return this->CVideoTrackingTask::getProgressSteps();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CVideoTrackingTaskWrap::getProgressSteps(size_t unitEltCount)
{
    CPyEnsureGIL gil;
    try
    {
        if(override getProgressStepsOver = this->get_override("getProgressSteps"))
            return getProgressStepsOver(unitEltCount);

        return CVideoTrackingTask::getProgressSteps(unitEltCount);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CVideoTrackingTaskWrap::default_getProgressSteps(size_t unitEltCount)
{
    CPyEnsureGIL gil;
    try
    {
        return this->CVideoTrackingTask::getProgressSteps(unitEltCount);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::setActive(bool bActive)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setActiveOver = this->get_override("setActive"))
            setActiveOver(bActive);
        else
            CVideoTrackingTask::setActive(bActive);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::default_setActive(bool bActive)
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoTrackingTask::setActive(bActive);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::updateStaticOutputs()
{
    CPyEnsureGIL gil;
    try
    {
        if(override updateStaticOutputsOver = this->get_override("updateStaticOutputs"))
            updateStaticOutputsOver();
        else
            CVideoTrackingTask::updateStaticOutputs();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::default_updateStaticOutputs()
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoTrackingTask::updateStaticOutputs();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::beginTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        if(override beginTaskRunOver = this->get_override("beginTaskRun"))
            beginTaskRunOver();
        else
            CVideoTrackingTask::beginTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::default_beginTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoTrackingTask::beginTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::endTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        if(override endTaskRunOver = this->get_override("endTaskRun"))
            endTaskRunOver();
        else
            CVideoTrackingTask::endTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::default_endTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoTrackingTask::endTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::run()
{
    CPyEnsureGIL gil;
    try
    {
        if(override runOver = this->get_override("run"))
            runOver();
        else
            CVideoTrackingTask::run();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::default_run()
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoTrackingTask::run();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::stop()
{
    CPyEnsureGIL gil;
    try
    {
        if(override stopOver = this->get_override("stop"))
            stopOver();
        else
            CVideoTrackingTask::stop();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::default_stop()
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoTrackingTask::stop();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::graphicsChanged()
{
    CPyEnsureGIL gil;
    try
    {
        if(override graphicsChangedOver = this->get_override("graphicsChanged"))
            graphicsChangedOver();
        else
            CVideoTrackingTask::graphicsChanged();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::default_graphicsChanged()
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoTrackingTask::graphicsChanged();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::globalInputChanged(bool bNewSequence)
{
    CPyEnsureGIL gil;
    try
    {
        if(override globalInputChangedOver = this->get_override("globalInputChanged"))
            globalInputChangedOver(bNewSequence);
        else
            CVideoTrackingTask::globalInputChanged(bNewSequence);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::default_globalInputChanged(bool bNewSequence)
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoTrackingTask::globalInputChanged(bNewSequence);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoTrackingTaskWrap::emitAddSubProgressSteps(int count)
{
    m_signalHandler->doAddSubTotalSteps(count);
}

void CVideoTrackingTaskWrap::emitGraphicsContextChanged()
{
    emit m_signalHandler->doGraphicsContextChanged();
}

void CVideoTrackingTaskWrap::emitStepProgress()
{
    emit m_signalHandler->doProgress();
}

void CVideoTrackingTaskWrap::emitOutputChanged()
{
    emit m_signalHandler->doOutputChanged();
}
