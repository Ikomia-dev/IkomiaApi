#ifndef CDATAIMAGEIO_H
#define CDATAIMAGEIO_H

#include <string>
#include <memory>
#include "CVirtualImageIO.h"
#include "DataIOGlobal.hpp"
/**
 * @brief
 *
 */
class DATAIOSHARED_EXPORT CDataImageIO
{
    using CImageIOPtr = std::unique_ptr<CVirtualImageIO>;

    public:

        /**
         * @brief
         *
         * @param fileName
         */
        CDataImageIO(const std::string& fileName);
        /**
         * @brief
         *
         * @return Dimensions
         */
        std::vector<std::string>        fileNames(const SubsetBounds& bounds);

        /**
         * @brief
         *
         * @return Dimensions
         */
        Dimensions                      dimensions();
        /**
         * @brief
         *
         * @return Dimensions
         */
        Dimensions                      dimensions(const SubsetBounds& bounds);
        /**
         * @brief
         *
         * @return CDataInfoPtr
         */
        CDataInfoPtr                    dataInfo();

        /**
         * @brief
         *
         * @return CDataInfoPtr
         */
        CDataInfoPtr                    dataInfo(const SubsetBounds& bounds);
        /**
         * @brief
         *
         * @return CMat
         */
        CMat                            read();
        /**
         * @brief
         *
         * @param subset
         * @return CMat
         */
        CMat                            read(const SubsetBounds& bounds);

        void                            write(const CMat &image);

        /**
         * @brief
         *
         * @param fileName
         * @return DataType
         */
        static IODataType               dataType(const std::string& fileName);

        static Dimensions               dimensions(const std::string& fileName);

        static std::set<std::string>    getOpencvExtensions();
        static bool                     isImageFormat(const std::string& extension);
        static bool                     isOpencvCompatible(const std::string& extension);

    private:

        void                            allocateDataIOPtr(const std::string& fileName);
        static CImageIOPtr              _allocateDataIOPtr(const std::string& fileName);

    private:

        CImageIOPtr         m_pImgIO;
};

#endif // CDATAIMAGEIO_H
