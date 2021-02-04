#ifndef CVIRTUALIMAGEIO_H
#define CVIRTUALIMAGEIO_H

#include <string>
#include "Data/CDataSubset.hpp"

/**
 * @brief
 *
 */
class CVirtualImageIO
{
    public:

        /**
         * @brief
         *
         * @param fileName
         */
        CVirtualImageIO(const std::string& fileName);
        /**
         * @brief
         *
         */
        virtual ~CVirtualImageIO();

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
        /**
         * @brief
         *
         * @param subset
         * @return CMat
         */
        virtual CMat            read(const SubsetBounds& subset) = 0;

        virtual void            write(const CMat& /*image*/){}

    protected:

        std::string             extension();

    protected:

        std::string m_fileName;
};

#endif // CVIRTUALIMAGEIO_H
