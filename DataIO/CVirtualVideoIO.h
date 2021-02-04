#ifndef CVIRTUALVIDEOIO_H
#define CVIRTUALVIDEOIO_H

#include <string>
#include "DataIOGlobal.hpp"
#include "Main/CoreDefine.hpp"
#include "Data/CMat.hpp"
#include "Data/CDataInfo.h"

/**
 * @brief
 *
 */
class DATAIOSHARED_EXPORT CVirtualVideoIO
{
    public:

        /**
         * @brief
         *
         * @param fileName
         */
        CVirtualVideoIO(const std::string& fileName);
        /**
         * @brief
         *
         */
        virtual ~CVirtualVideoIO();

        /**
         * @brief
         *
         * @return Dimensions
         */
        virtual VectorString    fileNames(const SubsetBounds& bounds) = 0;

        /**
         * @brief
         *
         * @return Dimensions
         */
        virtual Dimensions      dimensions() = 0;

        /**
         * @brief
         *
         * @return Dimensions
         */
        virtual Dimensions      dimensions(const SubsetBounds& subset) = 0;        
        /**
         * @brief
         *
         * @return CDataInfoPtr
         */
        virtual CDataInfoPtr    dataInfo() = 0;
        virtual CDataInfoPtr    dataInfo(const SubsetBounds& subset) = 0;
        /**
         * @brief
         *
         * @return CMat
         */
        virtual CMat            read() = 0;
        virtual CMat            readLive() = 0;
        virtual CMat            read(const SubsetBounds& subset) = 0;
        virtual CMat            readLive(const SubsetBounds& subset) = 0;

        virtual void            write(const CMat& /*image*/) = 0;
        virtual void            write(const CMat& /*image*/, const std::string& /*path*/) = 0;

        virtual void            stopWrite() = 0;
        virtual void            stopRead() = 0;

        virtual void            waitWriteFinished() = 0;

        virtual void            closeCamera();

    protected:

        std::string             extension();

    protected:

        std::string m_fileName;
};

#endif // CVIRTUALVIDEOIO_H
