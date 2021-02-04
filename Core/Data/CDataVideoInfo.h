#ifndef CDATAVIDEOINFO_H
#define CDATAVIDEOINFO_H

#include "CDataImageInfo.h"
#include "Main/CoreGlobal.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CDataVideoInfo : public CDataImageInfo
{
    public:

        //Constructors
        CDataVideoInfo();
        CDataVideoInfo(IODataType type);
        CDataVideoInfo(const std::string &fileName);
        CDataVideoInfo(IODataType type, const std::string &fileName);
        CDataVideoInfo(int fps, int frameCount, int currentPos, int fourcc);
        CDataVideoInfo(const CDataVideoInfo& info);
        CDataVideoInfo(CDataVideoInfo&& info);

        //Destructor
        virtual ~CDataVideoInfo();

        //Operators
        CDataVideoInfo& operator=(const CDataVideoInfo& info);
        CDataVideoInfo& operator=(CDataVideoInfo&& info);

        virtual VectorPairString    getStringList() const override;

    public:

        int     m_fps = 0;
        int     m_frameCount = 0;
        int     m_currentPos = 0;
        int     m_fourcc = 0;
};

/** @endcond */

#endif // CDATAVIDEOINFO_H
