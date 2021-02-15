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

#include "CProtocolTaskIOWrap.h"

CProtocolTaskIOWrap::CProtocolTaskIOWrap() : CProtocolTaskIO()
{
}

CProtocolTaskIOWrap::CProtocolTaskIOWrap(IODataType dataType) : CProtocolTaskIO(dataType)
{
}

CProtocolTaskIOWrap::CProtocolTaskIOWrap(const CProtocolTaskIO &io) : CProtocolTaskIO(io)
{
}

size_t CProtocolTaskIOWrap::getUnitElementCount() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getOver = this->get_override("getUnitElementCount"))
            return getOver();
        return CProtocolTaskIO::getUnitElementCount();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CProtocolTaskIOWrap::default_getUnitElementCount() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProtocolTaskIO::getUnitElementCount();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CProtocolTaskIOWrap::isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isDataOver = this->get_override("isDataAvailable"))
            return isDataOver();
        return CProtocolTaskIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CProtocolTaskIOWrap::default_isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProtocolTaskIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CProtocolTaskIOWrap::isAutoInput() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isAutoInputOver = this->get_override("isAutoInput"))
            return isAutoInputOver();
        return CProtocolTaskIO::isAutoInput();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CProtocolTaskIOWrap::default_isAutoInput() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProtocolTaskIO::isAutoInput();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskIOWrap::clearData()
{
    CPyEnsureGIL gil;
    try
    {
        if(override clearDataOver = this->get_override("clearData"))
            clearDataOver();
        else
            CProtocolTaskIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskIOWrap::default_clearData()
{
    CPyEnsureGIL gil;
    try
    {        
        this->CProtocolTaskIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskIOWrap::copyStaticData(const std::shared_ptr<CProtocolTaskIO> &ioPtr)
{
    CPyEnsureGIL gil;
    try
    {
        if(override copyOver = this->get_override("copyStaticData"))
            copyOver(ioPtr);
        else
            CProtocolTaskIO::copyStaticData(ioPtr);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskIOWrap::default_copyStaticData(const std::shared_ptr<CProtocolTaskIO> &ioPtr)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTaskIO::copyStaticData(ioPtr);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
