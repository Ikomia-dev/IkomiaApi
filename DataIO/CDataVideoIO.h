#ifndef CDATAVIDEOIO_H
#define CDATAVIDEOIO_H

#include <string>
#include <memory>
#include "CVirtualVideoIO.h"
#include "CDataVideoBuffer.h"

/**
 * @brief
 *
 */
class DATAIOSHARED_EXPORT CDataVideoIO
{
    using CVideoIOPtr = std::unique_ptr<CVirtualVideoIO>;

    public:

        CDataVideoIO();
        CDataVideoIO(const std::string& fileName);

        std::vector<std::string>        fileNames(const SubsetBounds& bounds);

        Dimensions                      dimensions();
        Dimensions                      dimensions(const SubsetBounds& bounds);

        CDataInfoPtr                    dataInfo();
        CDataInfoPtr                    dataInfo(const SubsetBounds& bounds);

        CMat                            read();
        CMat                            read(const SubsetBounds& bounds);
        CMat                            readLive();
        CMat                            readLive(const SubsetBounds& bounds);

        void                            write(const CMat &image);
        void                            write(const CMat &image, const std::string& path);

        void                            stopWrite();
        void                            stopRead();

        void                            waitWriteFinished();

        void                            closeCamera();

        static IODataType               dataType(const std::string& fileName);

        static Dimensions               dimensions(const std::string& fileName);

        static bool                     isVideoFormat(const std::string& extension, bool bVideoOnly);
        static bool                     isSameImageSequence(const std::string& fileRenamed, const std::string& fileName);

        static std::string              getFormattedSequenceName(const std::string& fileName);

    private:

        void                            allocateDataIOPtr(const std::string& fileName);
        static CVideoIOPtr              _allocateDataIOPtr(const std::string& fileName);

        static bool                     isImageFormat(const std::string& extension);

        static std::string              renameImageSequence(const std::string& fileName);

    private:

        CVideoIOPtr         m_pVideoIO;
};

#endif // CDATAVIDEOIO_H
