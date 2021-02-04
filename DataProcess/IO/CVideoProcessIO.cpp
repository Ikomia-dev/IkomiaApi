#include "CVideoProcessIO.h"
#include "Data/CDataVideoInfo.h"

CVideoProcessIO::CVideoProcessIO() : CImageProcessIO(IODataType::VIDEO)
{
    m_name = "CVideoProcessIO";
    m_description = QObject::tr("Video with read/write capabilities.").toStdString();
    m_saveFormat = DataFileFormat::AVI;
}

CVideoProcessIO::CVideoProcessIO(CMat image) : CImageProcessIO(IODataType::VIDEO, image)
{
    m_name = "CVideoProcessIO";
    m_description = QObject::tr("Video with read/write capabilities.").toStdString();
    m_saveFormat = DataFileFormat::AVI;
}

CVideoProcessIO::CVideoProcessIO(IODataType data) : CImageProcessIO(data)
{
    m_name = "CVideoProcessIO";
    m_description = QObject::tr("Video with read/write capabilities.").toStdString();
    m_saveFormat = DataFileFormat::AVI;
}

CVideoProcessIO::CVideoProcessIO(IODataType data, const CMat& image) : CImageProcessIO(data, image)
{
    m_name = "CVideoProcessIO";
    m_description = QObject::tr("Video with read/write capabilities.").toStdString();
    m_saveFormat = DataFileFormat::AVI;
}

CVideoProcessIO::CVideoProcessIO(const CVideoProcessIO &io) : CImageProcessIO(io)
{
}

CVideoProcessIO::CVideoProcessIO(const CVideoProcessIO&& io) : CImageProcessIO(io)
{
}

CVideoProcessIO &CVideoProcessIO::operator=(const CVideoProcessIO& io)
{
    CImageProcessIO::operator=(io);
    return *this;
}

CVideoProcessIO &CVideoProcessIO::operator=(const CVideoProcessIO&& io)
{
    CImageProcessIO::operator=(io);
    return *this;
}

void CVideoProcessIO::setVideoPath(const std::string& path)
{
    m_pVideoBuffer = std::make_unique<CDataVideoBuffer>(path);
}

void CVideoProcessIO::setVideoPos(int pos)
{
    if(m_pVideoBuffer)
        m_pVideoBuffer->setPosition(pos);
    else
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Video buffer pointer is null.").toStdString(), __func__, __FILE__, __LINE__);
}

void CVideoProcessIO::startVideo()
{
    if(m_pVideoBuffer)
        m_pVideoBuffer->startRead();
    else
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Video buffer pointer is null.").toStdString(), __func__, __FILE__, __LINE__);
}

void CVideoProcessIO::stopVideo()
{
    if(m_pVideoBuffer)
        m_pVideoBuffer->stopRead();
    else
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Video buffer pointer is null.").toStdString(), __func__, __FILE__, __LINE__);
}

void CVideoProcessIO::startVideoWrite(int width, int height, int frames, int fps, int fourcc)
{
    if(m_pVideoBuffer)
        m_pVideoBuffer->startWrite(width, height, frames, fps, fourcc);
    else
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Video buffer pointer is null.").toStdString(), __func__, __FILE__, __LINE__);
}

void CVideoProcessIO::stopVideoWrite()
{
    if(m_pVideoBuffer)
        m_pVideoBuffer->stopWrite();
    else
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Video buffer pointer is null.").toStdString(), __func__, __FILE__, __LINE__);
}

void CVideoProcessIO::addVideoImage(const CMat& image)
{
    m_videoImgList.push_back(image);
}

void CVideoProcessIO::writeImage(CMat image)
{
    assert(m_pVideoBuffer);
    CMat tmp;
    if(image.channels() == 1)
        cv::cvtColor(image, tmp, cv::COLOR_GRAY2BGR);
    else
        cv::cvtColor(image, tmp, cv::COLOR_RGB2BGR);

    m_pVideoBuffer->write(tmp);
}

bool CVideoProcessIO::hasVideo()
{
    return m_pVideoBuffer != nullptr;
}

CMat CVideoProcessIO::getImage() const
{
    if(m_pVideoBuffer && m_pVideoBuffer->isReadStarted())
    {
        CMat image = m_pVideoBuffer->read();
        return image;
    }
    else
        return m_image;
}

size_t CVideoProcessIO::getVideoFrameCount() const
{
    if(m_pVideoBuffer)
        return m_pVideoBuffer->getFrameCount();
    else
        return 0;
}

std::vector<CMat> CVideoProcessIO::getVideoImages() const
{
    return m_videoImgList;
}

std::string CVideoProcessIO::getVideoPath() const
{
    assert(m_pVideoBuffer);
    return m_pVideoBuffer->getCurrentPath();
}

CMat CVideoProcessIO::getSnapshot(int pos)
{
    assert(m_pVideoBuffer);
    return m_pVideoBuffer->snapshot(pos);
}

int CVideoProcessIO::getCurrentPos() const
{
    assert(m_pVideoBuffer);
    return m_pVideoBuffer->getCurrentPos();
}

CMat CVideoProcessIO::getStaticImage() const
{
    return m_image;
}

size_t CVideoProcessIO::getUnitElementCount() const
{
    if(m_pVideoBuffer)
        return m_pVideoBuffer->getFrameCount();
    else
        return 0;
}

CDataInfoPtr CVideoProcessIO::getDataInfo()
{
    if(m_pVideoBuffer)
    {
        m_infoPtr = std::make_shared<CDataVideoInfo>(m_pVideoBuffer->getFPS(),
                                                     m_pVideoBuffer->getFrameCount(),
                                                     m_pVideoBuffer->getCurrentPos(),
                                                     m_pVideoBuffer->getCodec());
        auto infoPtr = std::static_pointer_cast<CDataVideoInfo>(m_infoPtr);
        infoPtr->setFileName(m_pVideoBuffer->getCurrentPath());
        infoPtr->m_width = m_pVideoBuffer->getWidth();
        infoPtr->m_height = m_pVideoBuffer->getHeight();
    }
    return m_infoPtr;
}

bool CVideoProcessIO::isDataAvailable() const
{
    bool bRet =  m_image.data != nullptr || m_videoImgList.size() > 0 ;
    if(m_pVideoBuffer)
        bRet = bRet || m_pVideoBuffer->isReadOpened();

    return bRet;
}

void CVideoProcessIO::clearData()
{
    m_image.release();
}

void CVideoProcessIO::waitWriteFinished()
{
    m_pVideoBuffer->waitWriteFinished();
}

void CVideoProcessIO::waitReadImage() const
{
    if(m_pVideoBuffer)
        while(!m_pVideoBuffer->hasReadImage());
}

std::shared_ptr<CVideoProcessIO> CVideoProcessIO::clone() const
{
    return std::static_pointer_cast<CVideoProcessIO>(cloneImp());
}

void CVideoProcessIO::save()
{
    m_tempFiles.clear();
}

void CVideoProcessIO::save(const std::string &path)
{
    Q_UNUSED(path);
}

std::shared_ptr<CProtocolTaskIO> CVideoProcessIO::cloneImp() const
{
    return std::shared_ptr<CVideoProcessIO>(new CVideoProcessIO(*this));
}
