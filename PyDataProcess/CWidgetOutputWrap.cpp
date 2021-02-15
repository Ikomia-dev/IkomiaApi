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

#include "CWidgetOutputWrap.h"

CWidgetOutputWrap::CWidgetOutputWrap() : CWidgetOutput()
{
}

CWidgetOutputWrap::CWidgetOutputWrap(IODataType dataType) : CWidgetOutput(dataType)
{
}

CWidgetOutputWrap::CWidgetOutputWrap(const CWidgetOutput &out) : CWidgetOutput(out)
{
}

void CWidgetOutputWrap::setWidget(long long widgetPtr)
{
    CPyEnsureGIL gil;
    try
    {
        auto pWidget = reinterpret_cast<QWidget*>(widgetPtr);
        this->CWidgetOutput::setWidget(pWidget);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CWidgetOutputWrap::isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isDataOver = this->get_override("isDataAvailable"))
            return isDataOver();
        return CWidgetOutput::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CWidgetOutputWrap::default_isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CWidgetOutput::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWidgetOutputWrap::clearData()
{
    CPyEnsureGIL gil;
    try
    {
        if(override clearDataOver = this->get_override("clearData"))
            clearDataOver();
        else
            CWidgetOutput::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWidgetOutputWrap::default_clearData()
{
    CPyEnsureGIL gil;
    try
    {
        this->CWidgetOutput::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
