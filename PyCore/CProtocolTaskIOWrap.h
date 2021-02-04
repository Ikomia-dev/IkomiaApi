#ifndef CPROTOCOLTASKIOWRAP_HPP
#define CPROTOCOLTASKIOWRAP_HPP

#include "PyCoreGlobal.h"
#include "Protocol/CProtocolTaskIO.h"

class CProtocolTaskIOWrap : public CProtocolTaskIO, public wrapper<CProtocolTaskIO>
{
    public:

        CProtocolTaskIOWrap();
        CProtocolTaskIOWrap(IODataType dataType);
        CProtocolTaskIOWrap(const CProtocolTaskIO& io);

        virtual size_t  getUnitElementCount() const;
        size_t          default_getUnitElementCount() const;

        virtual bool    isDataAvailable() const;
        bool            default_isDataAvailable() const;

        virtual bool    isAutoInput() const;
        bool            default_isAutoInput() const;

        virtual void    clearData();
        void            default_clearData();

        virtual void    copyStaticData(const std::shared_ptr<CProtocolTaskIO>& ioPtr);
        void            default_copyStaticData(const std::shared_ptr<CProtocolTaskIO>& ioPtr);

};

#endif // CPROTOCOLTASKIOWRAP_HPP
