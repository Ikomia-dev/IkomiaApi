#ifndef CDATADICOMINFO_H
#define CDATADICOMINFO_H

#include "CDataImageInfo.h"
#include "Main/CoreGlobal.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CDataDicomInfo : public CDataImageInfo
{
    public:

        //Constructors
        CDataDicomInfo();
        CDataDicomInfo(const CDataDicomInfo& info);
        CDataDicomInfo(const CDataDicomInfo&& info);

        //Destructor
        virtual ~CDataDicomInfo();

        //Operators
        CDataDicomInfo&     operator=(const CDataDicomInfo& info);
        CDataDicomInfo&     operator=(const CDataDicomInfo&& info);

        VectorPairString    getStringList() const override;

    public:

        int     m_nbStudy = 0;
        int     m_nbSerie = 0;
};

/** @endcond */

#endif // CDATADICOMINFO_H
