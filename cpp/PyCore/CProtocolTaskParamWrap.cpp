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

#include "CProtocolTaskParamWrap.h"

CProtocolTaskParamWrap::CProtocolTaskParamWrap() : CProtocolTaskParam()
{
}

CProtocolTaskParamWrap::CProtocolTaskParamWrap(const CProtocolTaskParam &param)
    : CProtocolTaskParam(param)
{
}

void CProtocolTaskParamWrap::setParamMap(const UMapString &paramMap)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setParamOver = this->get_override("setParamMap"))
            setParamOver(paramMap);
        else
            CProtocolTaskParam::setParamMap(paramMap);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskParamWrap::default_setParamMap(const UMapString &paramMap)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTaskParam::setParamMap(paramMap);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

UMapString CProtocolTaskParamWrap::getParamMap() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getParamOver = this->get_override("getParamMap"))
            return getParamOver();
        return CProtocolTaskParam::getParamMap();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

UMapString CProtocolTaskParamWrap::default_getParamMap() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProtocolTaskParam::getParamMap();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

uint CProtocolTaskParamWrap::getHashValue() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getHashValueOver = this->get_override("getHashValue"))
            return getHashValueOver();
        return CProtocolTaskParam::getHashValue();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

uint CProtocolTaskParamWrap::default_getHashValue() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProtocolTaskParam::getHashValue();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
