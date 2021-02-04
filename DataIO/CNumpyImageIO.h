#ifndef CNUMPYIMAGEIO_H
#define CNUMPYIMAGEIO_H

#include "CVirtualImageIO.h"
#include "DataIOGlobal.hpp"

//Avoid conflict with Qt slots keyword
#undef slots
#include <Python.h>
#define slots

#include "boost/python.hpp"


class DATAIOSHARED_EXPORT CNumpyImageIO : public CVirtualImageIO
{
    public:

        CNumpyImageIO(const std::string& fileName);

        virtual ~CNumpyImageIO();

        VectorString    fileNames(const SubsetBounds& bounds) override;

        Dimensions      dimensions() override;
        Dimensions      dimensions(const SubsetBounds& bounds) override;

        CDataInfoPtr    dataInfo() override;
        CDataInfoPtr    dataInfo(const SubsetBounds& bounds) override;

        CMat            read() override;
        CMat            read(const SubsetBounds& bounds) override;

    private:

        //bool            initNumpy();
        void            loadNumpyArray();

    private:

        std::vector<int>    m_dims;
        std::vector<CMat>   m_images;
};

#endif // CNUMPYIMAGEIO_H
