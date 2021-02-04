#include "CVirtualImageIO.h"
#include <boost/filesystem.hpp>

CVirtualImageIO::CVirtualImageIO(const std::string& fileName)
{
    m_fileName = fileName;
}

CVirtualImageIO::~CVirtualImageIO()
{
}

std::string CVirtualImageIO::extension()
{
    boost::filesystem::path file(m_fileName);
    std::string ext = file.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}
