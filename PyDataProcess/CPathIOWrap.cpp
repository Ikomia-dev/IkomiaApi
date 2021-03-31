#include "CPathIOWrap.h"

CPathIOWrap::CPathIOWrap() : CPathIO()
{
}

CPathIOWrap::CPathIOWrap(IODataType dataType) : CPathIO(dataType)
{
}

CPathIOWrap::CPathIOWrap(IODataType dataType, const std::string &path) : CPathIO(dataType, path)
{
}

CPathIOWrap::CPathIOWrap(const CPathIO &io) : CPathIO(io)
{
}

bool CPathIOWrap::isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isDataOver = this->get_override("isDataAvailable"))
            return isDataOver();
        return CPathIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CPathIOWrap::default_isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CPathIO::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CPathIOWrap::clearData()
{
    CPyEnsureGIL gil;
    try
    {
        if(override clearDataOver = this->get_override("clearData"))
            clearDataOver();
        else
            CPathIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CPathIOWrap::default_clearData()
{
    CPyEnsureGIL gil;
    try
    {
        this->CPathIO::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
