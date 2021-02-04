#include "CVirtualVideoIO.h"
#include <boost/filesystem.hpp>

CVirtualVideoIO::CVirtualVideoIO(const std::string& fileName)
{
    m_fileName = fileName;
}

CVirtualVideoIO::~CVirtualVideoIO()
{
}

void CVirtualVideoIO::closeCamera()
{
    throw CException(CoreExCode::NOT_IMPLEMENTED, "Video IO: function not implemented");
}

std::string CVirtualVideoIO::extension()
{
    boost::filesystem::path file(m_fileName);
    std::string ext = file.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}
