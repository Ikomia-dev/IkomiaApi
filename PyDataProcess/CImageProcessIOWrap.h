#ifndef CIMAGRPROCESSIOWRAP_H
#define CIMAGRPROCESSIOWRAP_H

#include "PyDataProcessGlobal.h"
#include "IO/CImageProcessIO.h"

class CImageProcessIOWrap : public CImageProcessIO, public wrapper<CImageProcessIO>
{
    public:

        CImageProcessIOWrap();
        CImageProcessIOWrap(const CMat& image);
        CImageProcessIOWrap(IODataType dataType);
        CImageProcessIOWrap(IODataType dataType, const CMat& image);
        CImageProcessIOWrap(const CImageProcessIO &io);

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

#endif // CIMAGRPROCESSIOWRAP_H
