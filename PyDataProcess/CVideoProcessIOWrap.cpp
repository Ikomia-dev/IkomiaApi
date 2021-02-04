#include "CVideoProcessIOWrap.h"

CVideoProcessIOWrap::CVideoProcessIOWrap() : CVideoProcessIO()
{
}

CVideoProcessIOWrap::CVideoProcessIOWrap(const CMat &image) : CVideoProcessIO(image)
{
}

CVideoProcessIOWrap::CVideoProcessIOWrap(IODataType dataType) : CVideoProcessIO(dataType)
{
}

CVideoProcessIOWrap::CVideoProcessIOWrap(IODataType dataType, const CMat &image) : CVideoProcessIO(dataType, image)
{
}

CVideoProcessIOWrap::CVideoProcessIOWrap(const CVideoProcessIO &io) : CVideoProcessIO(io)
{
}

size_t CVideoProcessIOWrap::getUnitElementCount() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getOver = this->get_override("getUnitElementCount"))
            return getOver();
        return CVideoProcessIO::getUnitElementCount();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CVideoProcessIOWrap::default_getUnitElementCount() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CVideoProcessIO::getUnitElementCount();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

CMat CVideoProcessIOWrap::getImage() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getImageOver = this->get_override("getImage"))
            return getImageOver();
        return CVideoProcessIO::getImage();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

CMat CVideoProcessIOWrap::default_getImage() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CVideoProcessIO::getImage();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CVideoProcessIOWrap::isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isDataOver = this->get_override("isDataAvailable"))
            return isDataOver();
        return CVideoProcessIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CVideoProcessIOWrap::default_isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CVideoProcessIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoProcessIOWrap::clearData()
{
    CPyEnsureGIL gil;
    try
    {
        if(override clearDataOver = this->get_override("clearData"))
            clearDataOver();
        else
            CVideoProcessIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoProcessIOWrap::default_clearData()
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoProcessIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoProcessIOWrap::copyStaticData(const std::shared_ptr<CProtocolTaskIO> &ioPtr)
{
    CPyEnsureGIL gil;
    try
    {
        if(override copyOver = this->get_override("copyStaticData"))
            copyOver(ioPtr);
        else
            CVideoProcessIO::copyStaticData(ioPtr);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CVideoProcessIOWrap::default_copyStaticData(const std::shared_ptr<CProtocolTaskIO> &ioPtr)
{
    CPyEnsureGIL gil;
    try
    {
        this->CVideoProcessIO::copyStaticData(ioPtr);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
