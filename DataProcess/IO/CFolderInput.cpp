#include <QObject>
#include "CFolderInput.h"

CFolderInput::CFolderInput() : CProtocolTaskIO(IODataType::FOLDER)
{
    m_name = "CFolderInput";
    m_description = QObject::tr("File system path (folder only).\nUsed as input only.").toStdString();
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

CFolderInput::CFolderInput(const std::string &path) : CProtocolTaskIO(IODataType::FOLDER)
{
    m_name = "CFolderInput";
    m_description = QObject::tr("File system path (folder only).\nUsed as input only.").toStdString();
    m_path = path;
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

void CFolderInput::setPath(const std::string &path)
{
    assert(m_infoPtr);
    m_path = path;
    m_infoPtr->setFileName(path);
}

std::string CFolderInput::getPath() const
{
    return m_path;
}

bool CFolderInput::isDataAvailable() const
{
    return true;
}

void CFolderInput::clearData()
{
    m_path.clear();
}

std::shared_ptr<CProtocolTaskIO> CFolderInput::clone() const
{
    return std::static_pointer_cast<CFolderInput>(cloneImp());
}

std::shared_ptr<CProtocolTaskIO> CFolderInput::cloneImp() const
{
    return std::shared_ptr<CFolderInput>(new CFolderInput(*this));
}
