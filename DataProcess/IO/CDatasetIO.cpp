#include "CDatasetIO.h"
#include <boost/algorithm/string.hpp>

std::set<std::string> CDatasetIO::m_formats = { "other" };

CDatasetIO::CDatasetIO() : CProtocolTaskIO(IODataType::DNN_DATASET)
{
    m_srcFormat = "other";
    m_description = QObject::tr("Annotated image dataset in Ikomia format (for Deep Learning).").toStdString();
    m_saveFormat = DataFileFormat::JSON;
}

CDatasetIO::CDatasetIO(const std::string& srcFormat) : CProtocolTaskIO(IODataType::DNN_DATASET)
{
    const std::string srcFormatTmp = boost::algorithm::to_lower_copy(srcFormat);
    auto it = m_formats.find(srcFormatTmp);

    if(it == m_formats.end())
        m_formats.insert(srcFormatTmp);

    m_srcFormat = srcFormatTmp;
    m_description = QObject::tr("Annotated image dataset in Ikomia format (for Deep Learning).").toStdString();
    m_saveFormat = DataFileFormat::JSON;
}

std::string CDatasetIO::getSourceFormat() const
{
    return m_srcFormat;
}

std::vector<std::string> CDatasetIO::getImagePaths() const
{
    return std::vector<std::string>();
}

MapIntStr CDatasetIO::getCategories() const
{
    return MapIntStr();
}

int CDatasetIO::getCategoryCount() const
{
    return 0;
}

std::vector<ProxyGraphicsItemPtr> CDatasetIO::getAnnotationGraphics(const std::string &imgPath) const
{
    Q_UNUSED(imgPath);
    return std::vector<ProxyGraphicsItemPtr>();
}

std::string CDatasetIO::getMaskPath(const std::string &imgPath) const
{
    Q_UNUSED(imgPath);
    return "";
}

QJsonDocument CDatasetIO::getJsonDocument() const
{
    return m_json;
}

bool CDatasetIO::isDataAvailable() const
{
    return false;
}

bool CDatasetIO::isAutoInput() const
{
    return true;
}

void CDatasetIO::clearData()
{
}

void CDatasetIO::save(const std::string &path)
{
    if(m_json.isNull())
        return;

    QFile jsonFile(QString::fromStdString(path));
    if(jsonFile.open(QFile::WriteOnly | QFile::Text))
        jsonFile.write(m_json.toJson());
}

bool CDatasetIO::load(const std::string &path)
{
    QFile jsonFile(QString::fromStdString(path));
    if(jsonFile.open(QFile::ReadOnly | QFile::Text) == false)
        return false;

    m_json = QJsonDocument::fromJson(jsonFile.readAll());
    jsonFile.close();
    return true;
}
