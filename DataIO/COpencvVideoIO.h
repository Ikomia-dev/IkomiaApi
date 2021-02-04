#ifndef COPENCVVIDEOIO_H
#define COPENCVVIDEOIO_H

#include <string>
#include "CVirtualVideoIO.h"
#include "CDataVideoBuffer.h"

class DATAIOSHARED_EXPORT COpencvVideoIO : public CVirtualVideoIO
{
    public:

        COpencvVideoIO(const std::string& fileName);

        virtual ~COpencvVideoIO();

        VectorString    fileNames(const SubsetBounds& bounds) override;

        Dimensions      dimensions() override;
        Dimensions      dimensions(const SubsetBounds& bounds) override;

        CDataInfoPtr    dataInfo() override;
        CDataInfoPtr    dataInfo(const SubsetBounds& subset) override;

        CMat            read() override;
        CMat            read(const SubsetBounds& subset) override;
        CMat            readLive() override;
        CMat            readLive(const SubsetBounds& subset) override;

        void            write(const CMat& image) override;
        void            write(const CMat& image, const std::string& path) override;

        void            stopWrite() override;
        void            stopRead() override;

        void            waitWriteFinished() override;

        void            closeCamera() override;

    private:

        cv::VideoCapture    m_cap;
        cv::VideoWriter     m_out;
        CDataVideoBufferPtr m_pVideoBuffer = nullptr;
        // IMAGE
        int                 m_imgWidth = 0;
        int                 m_imgHeight = 0;
        /*int                 m_imgDepth = 0;
        int                 m_imgNbband = 0;
        int                 m_imgNbBit = 0;
        int                 m_imgBitMin = 0;
        int                 m_imgBitMax = 0;*/
        int                 m_imgCvType = 0;

        // VIDEO
        int                 m_width = 0;
        int                 m_height = 0;
        int                 m_fps = 0;
        int                 m_frameCount = 0;
        //int                 m_currentPos = 0;
        int                 m_fourcc = 0;
};
#endif // COPENCVVIDEOIO_H
