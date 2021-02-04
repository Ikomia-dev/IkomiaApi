#ifndef CDATAIMAGEINFO_H
#define CDATAIMAGEINFO_H

#include "CDataInfo.h"
#include "Main/CoreGlobal.hpp"
#include "Data/CMat.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CDataImageInfo : public CDataInfo
{
    public:

        //Constructors
        CDataImageInfo();
        CDataImageInfo(IODataType type);
        CDataImageInfo(const std::string& fileName);
        CDataImageInfo(IODataType type, const std::string& fileName);
        CDataImageInfo(const CMat& image);
        CDataImageInfo(const CDataImageInfo& info);
        CDataImageInfo(CDataImageInfo&& info);

        //Destructor
        virtual ~CDataImageInfo();

        //Operators
        CDataImageInfo&             operator=(const CDataImageInfo& info);
        CDataImageInfo&             operator=(CDataImageInfo&& info);

        void                        setCvType(int getType);

        virtual VectorPairString    getStringList() const override;

        void                        updateImage(const CMat& image);

    private:

        std::string                 getTypeString() const;

        void                        computeMinMax(const CMat& image);

    public:

        int     m_width = 0;
        int     m_height = 0;
        int     m_depth = 0;
        int     m_nbBand = 0;
        int     m_nbBit = 0;
        int     m_bitMin = 0;
        int     m_bitMax = 0;
        int     m_cvType = CV_8U;
        double  m_minValue = 0;
        double  m_maxValue = 255;
};

/** @endcond */

#endif // CDATAIMAGEINFO_H
