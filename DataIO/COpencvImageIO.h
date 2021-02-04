#ifndef COPENCVIMAGEIO_H
#define COPENCVIMAGEIO_H

#include <string>
#include "CVirtualImageIO.h"
#include "DataIOGlobal.hpp"

/**
 * @brief
 *
 */
class DATAIOSHARED_EXPORT COpencvImageIO : public CVirtualImageIO
{
    public:

        COpencvImageIO(const std::string& fileName);

        virtual ~COpencvImageIO();

        VectorString    fileNames(const SubsetBounds& bounds) override;

        Dimensions      dimensions() override;
        Dimensions      dimensions(const SubsetBounds& bounds) override;

        CDataInfoPtr    dataInfo() override;
        CDataInfoPtr    dataInfo(const SubsetBounds& subset) override;

        CMat            read() override;
        CMat            read(const SubsetBounds& subset) override;

        void            write(const CMat& image) override;

    private:

        void            writeSingle(const std::string& path, const CMat& image);
};

#endif // COPENCVIMAGEIO_H
