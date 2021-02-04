#include "CException.h"

CException::CException() : std::exception()
{
    m_msgType = 0;
    m_msg = formatMessage();
}

CException::CException(int code, const std::string &error): std::exception()
{
    m_code = code;
    m_error = error;
    m_msgType = 1;
    m_msg = formatMessage();

}

CException::CException(int code, const std::string &error, const std::string &func, const std::string &file, int line): std::exception()
{
    m_code = code;
    m_error = error;
    m_func = func;
    m_file = file;
    m_line = line;
    m_msgType = 2;
    m_msg = formatMessage();
}

CException::CException(int code, const cv::Exception &e, const std::string &func, const std::string &file, int line): std::exception()
{
#ifdef QT_DEBUG
    m_error = e.what();
#else
    m_error = "OpenCV:" + e.err;
#endif

    m_code = code;
    m_func = func;
    m_file = file;
    m_line = line;
    m_msgType = 2;
    m_msg = formatMessage();
}

CException::~CException()
{
}

const char *CException::what() const noexcept
{
    return m_msg.c_str();
}

std::string CException::getMessage() const
{
    return formatMessage();
}

int CException::getCode() const
{
    return m_code;
}

std::string CException::getError() const
{
    return m_error;
}

std::string CException::formatMessage() const
{
    std::string msg;
    std::string strCode = std::to_string(m_code);

    switch(m_msgType)
    {
        case 0:
            msg = "Ikomia error";
            break;
        case 1:
            msg = m_error + " (Code " + strCode + ")";
            break;
        case 2:
#ifdef QT_DEBUG
            auto strFile = formatFile();
            std::string strLine = std::to_string(m_line);
            msg = strFile + ":" + strLine + ": " + m_error + " in function " + "'" +  m_func + "'" + " (Code " + strCode + ")";
#else
            msg = m_error + " (Code " + strCode + ")";
#endif
            break;
    }
    return msg;
}

std::string CException::formatFile() const
{
    if(m_file.empty() == true)
        return "";

    size_t pos = m_file.rfind('/');
    if(pos == std::string::npos)
        pos = m_file.rfind('\\');

    if(pos == std::string::npos)
        return "";

    auto strFile = m_file.substr(pos + 1, std::string::npos);
    return strFile;
}

