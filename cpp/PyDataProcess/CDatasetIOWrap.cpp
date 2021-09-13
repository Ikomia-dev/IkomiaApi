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

#include "CDatasetIOWrap.h"

CDatasetIOWrap::CDatasetIOWrap() : CDatasetIO()
{
}

CDatasetIOWrap::CDatasetIOWrap(const std::string& name): CDatasetIO(name)
{
}

CDatasetIOWrap::CDatasetIOWrap(const std::string& name, const std::string &sourceFormat): CDatasetIO(name, sourceFormat)
{
}

std::vector<std::string> CDatasetIOWrap::getImagePaths() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override overMeth = this->get_override("getImagePaths"))
            return overMeth();
        else
            return CDatasetIO::getImagePaths();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

std::vector<std::string> CDatasetIOWrap::default_getImagePaths() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CDatasetIO::getImagePaths();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

MapIntStr CDatasetIOWrap::getCategories() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override overMeth = this->get_override("getCategories"))
            return overMeth();
        else
            return CDatasetIO::getCategories();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

MapIntStr CDatasetIOWrap::default_getCategories() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CDatasetIO::getCategories();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

int CDatasetIOWrap::getCategoryCount() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override overMeth = this->get_override("getCategoryCount"))
            return overMeth();
        else
            return CDatasetIO::getCategoryCount();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

int CDatasetIOWrap::default_getCategoryCount() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CDatasetIO::getCategoryCount();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

std::vector<ProxyGraphicsItemPtr> CDatasetIOWrap::getGraphicsAnnotations(const std::string &imgPath) const
{
    CPyEnsureGIL gil;
    try
    {
        if(override overMeth = this->get_override("getGraphicsAnnotations"))
            return overMeth(imgPath);
        else
            return CDatasetIO::getGraphicsAnnotations(imgPath);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

std::vector<ProxyGraphicsItemPtr> CDatasetIOWrap::default_getGraphicsAnnotations(const std::string &imgPath) const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CDatasetIO::getGraphicsAnnotations(imgPath);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

std::string CDatasetIOWrap::getMaskPath(const std::string &imgPath) const
{
    CPyEnsureGIL gil;
    try
    {
        if(override overMeth = this->get_override("getMaskPath"))
            return overMeth(imgPath);
        else
            return CDatasetIO::getMaskPath(imgPath);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

std::string CDatasetIOWrap::default_getMaskPath(const std::string &imgPath) const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CDatasetIO::getMaskPath(imgPath);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CDatasetIOWrap::isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isDataOver = this->get_override("isDataAvailable"))
            return isDataOver();
        return CDatasetIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CDatasetIOWrap::default_isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CDatasetIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CDatasetIOWrap::clearData()
{
    CPyEnsureGIL gil;
    try
    {
        if(override clearDataOver = this->get_override("clearData"))
            clearDataOver();
        else
            CDatasetIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CDatasetIOWrap::default_clearData()
{
    CPyEnsureGIL gil;
    try
    {
        this->CDatasetIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CDatasetIOWrap::save(const std::string &path)
{
    CPyEnsureGIL gil;
    try
    {
        if(override overMeth = this->get_override("save"))
            overMeth(path);
        else
            CDatasetIO::save(path);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CDatasetIOWrap::default_save(const std::string &path)
{
    CPyEnsureGIL gil;
    try
    {
        this->CDatasetIO::save(path);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}


