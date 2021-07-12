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

#include "CWorkflowTaskIOWrap.h"

CWorkflowTaskIOWrap::CWorkflowTaskIOWrap() : CWorkflowTaskIO()
{
}

CWorkflowTaskIOWrap::CWorkflowTaskIOWrap(IODataType dataType) : CWorkflowTaskIO(dataType)
{
}

CWorkflowTaskIOWrap::CWorkflowTaskIOWrap(IODataType dataType, const std::string &name) : CWorkflowTaskIO(dataType, name)
{
}

CWorkflowTaskIOWrap::CWorkflowTaskIOWrap(const CWorkflowTaskIO &io) : CWorkflowTaskIO(io)
{
}

size_t CWorkflowTaskIOWrap::getUnitElementCount() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getOver = this->get_override("getUnitElementCount"))
            return getOver();
        return CWorkflowTaskIO::getUnitElementCount();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CWorkflowTaskIOWrap::default_getUnitElementCount() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CWorkflowTaskIO::getUnitElementCount();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CWorkflowTaskIOWrap::isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isDataOver = this->get_override("isDataAvailable"))
            return isDataOver();
        return CWorkflowTaskIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CWorkflowTaskIOWrap::default_isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CWorkflowTaskIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CWorkflowTaskIOWrap::isAutoInput() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isAutoInputOver = this->get_override("isAutoInput"))
            return isAutoInputOver();
        return CWorkflowTaskIO::isAutoInput();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CWorkflowTaskIOWrap::default_isAutoInput() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CWorkflowTaskIO::isAutoInput();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWorkflowTaskIOWrap::clearData()
{
    CPyEnsureGIL gil;
    try
    {
        if(override clearDataOver = this->get_override("clearData"))
            clearDataOver();
        else
            CWorkflowTaskIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWorkflowTaskIOWrap::default_clearData()
{
    CPyEnsureGIL gil;
    try
    {        
        this->CWorkflowTaskIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWorkflowTaskIOWrap::copyStaticData(const std::shared_ptr<CWorkflowTaskIO> &ioPtr)
{
    CPyEnsureGIL gil;
    try
    {
        if(override copyOver = this->get_override("copyStaticData"))
            copyOver(ioPtr);
        else
            CWorkflowTaskIO::copyStaticData(ioPtr);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWorkflowTaskIOWrap::default_copyStaticData(const std::shared_ptr<CWorkflowTaskIO> &ioPtr)
{
    CPyEnsureGIL gil;
    try
    {
        this->CWorkflowTaskIO::copyStaticData(ioPtr);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
