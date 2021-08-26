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

#ifndef CNUMERICIOWRAP_H
#define CNUMERICIOWRAP_H

#include "PyDataProcessGlobal.h"
#include "IO/CNumericIO.hpp"

template<class Type>
class CNumericIOWrap : public CNumericIO<Type>, public wrapper<CNumericIO<Type>>
{
    public:

        CNumericIOWrap() : CNumericIO<Type>()
        {
        }
        CNumericIOWrap(const CNumericIO<Type>& io) : CNumericIO<Type>(io)
        {
        }

        virtual size_t  getUnitElementCount() const
        {
            CPyEnsureGIL gil;
            try
            {
                if(override getOver = this->get_override("getUnitElementCount"))
                    return getOver();
                return CNumericIO<Type>::getUnitElementCount();
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }
        size_t          default_getUnitElementCount() const
        {
            CPyEnsureGIL gil;
            try
            {
                return this->CNumericIO<Type>::getUnitElementCount();
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }

        virtual bool    isDataAvailable() const
        {
            CPyEnsureGIL gil;
            try
            {
                if(override isDataOver = this->get_override("isDataAvailable"))
                    return isDataOver();
                return CNumericIO<Type>::isDataAvailable();
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }
        bool            default_isDataAvailable() const
        {
            CPyEnsureGIL gil;
            try
            {
                return this->CNumericIO<Type>::isDataAvailable();
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }

        virtual void    clearData()
        {
            CPyEnsureGIL gil;
            try
            {
                if(override clearDataOver = this->get_override("clearData"))
                    clearDataOver();
                else
                    CNumericIO<Type>::clearData();
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }
        void            default_clearData()
        {
            CPyEnsureGIL gil;
            try
            {
                this->CNumericIO<Type>::clearData();
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }

        virtual void    copyStaticData(const std::shared_ptr<CWorkflowTaskIO>& ioPtr)
        {
            CPyEnsureGIL gil;
            try
            {
                if(override copyOver = this->get_override("copyStaticData"))
                    copyOver(ioPtr);
                else
                    CNumericIO<Type>::copyStaticData(ioPtr);
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }
        void            default_copyStaticData(const std::shared_ptr<CWorkflowTaskIO>& ioPtr)
        {
            CPyEnsureGIL gil;
            try
            {
                this->CNumericIO<Type>::copyStaticData(ioPtr);
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }
};

#endif // CNUMERICIOWRAP_H
