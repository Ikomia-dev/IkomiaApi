#ifndef CDATACONVERSION_H
#define CDATACONVERSION_H

#include "opencv2/opencv.hpp"
#include <QImage>
#include "CMat.hpp"
#include "Main/CoreGlobal.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CDataConversion
{
    public:

        CDataConversion();

        static QImage   CMatToQImage(CMat imgSrc, bool bCopy = false);
        static void     grayscaleToAlpha(const CMat& src, CMat& dst);

    private:

        static void     to8Bits(const CMat& src, CMat& dst);
};

/** @endcond */

#endif // CDATACONVERSION_H
