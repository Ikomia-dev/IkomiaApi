#include <QObject>
#include <boost/filesystem.hpp>
#include "CPathIO.h"
#include "CFolderInput.h"

CPathIO::CPathIO() : CProtocolTaskIO(IODataType::FILE_PATH)
{
    m_name = "CPathIO";
    m_description = QObject::tr("File system path (file or folder).").toStdString();
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

CPathIO::CPathIO(IODataType data) : CProtocolTaskIO(data)
{
    m_name = "CPathIO";
    m_description = QObject::tr("File system path (file or folder).").toStdString();
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

CPathIO::CPathIO(IODataType data, const std::string &path) : CProtocolTaskIO(data)
{
    m_name = "CPathIO";
    m_description = QObject::tr("File system path (file or folder).").toStdString();
    m_path = path;
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

CPathIO::CPathIO(const CFolderInput &in) : CProtocolTaskIO(in)
{
    m_name = "CPathIO";
    m_path = in.getPath();
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

void CPathIO::setPath(const std::string &path)
{
    assert(m_infoPtr);
    m_path = path;
    m_infoPtr->setFileName(path);
}

std::string CPathIO::getPath() const
{
    return m_path;
}

bool CPathIO::isDataAvailable() const
{
    boost::filesystem::path boostPath(m_path);
    return boost::filesystem::exists(boostPath);
}

bool CPathIO::isAutoInput() const
{
    return true;
}

void CPathIO::clearData()
{
    m_path.clear();
}

void CPathIO::copy(const std::shared_ptr<CProtocolTaskIO> &ioPtr)
{
    auto pFolderInput = dynamic_cast<const CFolderInput*>(ioPtr.get());
    if(pFolderInput)
        *this = *pFolderInput;
}

std::shared_ptr<CProtocolTaskIO> CPathIO::clone() const
{
    return std::static_pointer_cast<CPathIO>(cloneImp());
}

std::shared_ptr<CProtocolTaskIO> CPathIO::cloneImp() const
{
    return std::shared_ptr<CPathIO>(new CPathIO(*this));
}
