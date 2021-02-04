#ifndef CFEATUREPROCESSIOWRAP_H
#define CFEATUREPROCESSIOWRAP_H

#include "PyDataProcessGlobal.h"
#include "IO/CFeatureProcessIO.hpp"

template<class Type>
class CFeatureProcessIOWrap : public CFeatureProcessIO<Type>, public wrapper<CFeatureProcessIO<Type>>
{
    public:

        CFeatureProcessIOWrap() : CFeatureProcessIO<Type>()
        {
        }
        CFeatureProcessIOWrap(const CFeatureProcessIO<Type>& io) : CFeatureProcessIO<Type>(io)
        {
        }

        virtual size_t  getUnitElementCount() const
        {
            CPyEnsureGIL gil;
            try
            {
                if(override getOver = this->get_override("getUnitElementCount"))
                    return getOver();
                return CFeatureProcessIO<Type>::getUnitElementCount();
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
                return this->CFeatureProcessIO<Type>::getUnitElementCount();
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
                return CFeatureProcessIO<Type>::isDataAvailable();
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
                return this->CFeatureProcessIO<Type>::isDataAvailable();
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
                    CFeatureProcessIO<Type>::clearData();
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
                this->CFeatureProcessIO<Type>::clearData();
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }

        virtual void    copyStaticData(const std::shared_ptr<CProtocolTaskIO>& ioPtr)
        {
            CPyEnsureGIL gil;
            try
            {
                if(override copyOver = this->get_override("copyStaticData"))
                    copyOver(ioPtr);
                else
                    CFeatureProcessIO<Type>::copyStaticData(ioPtr);
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }
        void            default_copyStaticData(const std::shared_ptr<CProtocolTaskIO>& ioPtr)
        {
            CPyEnsureGIL gil;
            try
            {
                this->CFeatureProcessIO<Type>::copyStaticData(ioPtr);
            }
            catch(boost::python::error_already_set&)
            {
                throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
            }
        }
};

#endif // CFEATUREPROCESSIOWRAP_H
