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

#include "CVideoIOWrap.h"

CVideoIOWrap::CVideoIOWrap() : CVideoIO()
{
}

CVideoIOWrap::CVideoIOWrap(IODataType dataType) : CVideoIO(dataType)
{
}

CVideoIOWrap::CVideoIOWrap(IODataType dataType, const CMat &image) : CVideoIO(dataType, image)
{
}

CVideoIOWrap::CVideoIOWrap(IODataType dataType, const CMat &image, const std::string& name) : CVideoIO(dataType, image, name)
{
}

CVideoIOWrap::CVideoIOWrap(const CVideoIO &io) : CVideoIO(io)
{
}

size_t CVideoIOWrap::getUnitElementCount() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getOver = this->get_override("getUnitElementCount"))
            return getOver();
        return CVideoIO::getUnitElementCount();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CVideoIOWrap::default_getUnitElementCount() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CVideoIO::getUnitElementCount();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

CMat CVideoIOWrap::getImage()
{
    CPyEnsureGIL gil;
    try
    {
        if(override getImageOver = this->get_override("getImage"))
            return getImageOver();
        return CVideoIO::getImage();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

CMat CVideoIOWrap::default_getImage()
{
    CPyEnsureGIL gil;
    try
    {
        return this->CVideoIO::getImage();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CVideoIOWrap::isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isDataOver = this->get_override("isDataAvailable"))
            return isDataOver();
        return CVideoIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CVideoIOWrap::default_isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CVideoIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoIOWrap::clearData()
{
    CPyEnsureGIL gil;
    try
    {
        if(override clearDataOver = this->get_override("clearData"))
            clearDataOver();
        else
            CVideoIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoIOWrap::default_clearData()
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoIOWrap::copyStaticData(const std::shared_ptr<CWorkflowTaskIO> &ioPtr)
{
    CPyEnsureGIL gil;
    try
    {
        if(override copyOver = this->get_override("copyStaticData"))
            copyOver(ioPtr);
        else
            CVideoIO::copyStaticData(ioPtr);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoIOWrap::default_copyStaticData(const std::shared_ptr<CWorkflowTaskIO> &ioPtr)
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoIO::copyStaticData(ioPtr);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
