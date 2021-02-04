#ifndef CVIDEOPROCESSIOWRAP_H
#define CVIDEOPROCESSIOWRAP_H

#include "PyDataProcessGlobal.h"
#include "IO/CVideoProcessIO.h"

class CVideoProcessIOWrap : public CVideoProcessIO, public wrapper<CVideoProcessIO>
{
    public:

        CVideoProcessIOWrap();
        CVideoProcessIOWrap(const CMat& image);
        CVideoProcessIOWrap(IODataType dataType);
        CVideoProcessIOWrap(IODataType dataType, const CMat& image);
        CVideoProcessIOWrap(const CVideoProcessIO &io);

        virtual size_t  getUnitElementCount() const;
        size_t          default_getUnitElementCount() const;

        virtual CMat    getImage() const;
        CMat            default_getImage() const;

        virtual bool    isDataAvailable() const;
        bool            default_isDataAvailable() const;

        virtual void    clearData();
        void            default_clearData();

        virtual void    copyStaticData(const std::shared_ptr<CProtocolTaskIO>& ioPtr);
        void            default_copyStaticData(const std::shared_ptr<CProtocolTaskIO>& ioPtr);
};

#endif // CVIDEOPROCESSIOWRAP_H
