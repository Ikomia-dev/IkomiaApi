#ifndef CPROTOCOLTASKPARAMWRAP_HPP
#define CPROTOCOLTASKPARAMWRAP_HPP

#include "PyCoreGlobal.h"
#include "Protocol/CProtocolTaskParam.h"

class CProtocolTaskParamWrap : public CProtocolTaskParam, public wrapper<CProtocolTaskParam>
{
    public:

        CProtocolTaskParamWrap();
        CProtocolTaskParamWrap(const CProtocolTaskParam& param);

        virtual void        setParamMap(const UMapString &paramMap);
        void                default_setParamMap(const UMapString& paramMap);

        virtual UMapString  getParamMap() const;
        UMapString          default_getParamMap() const;

        virtual uint        getHashValue() const;
        uint                default_getHashValue() const;
};

#endif // CPROTOCOLTASKPARAMWRAP_HPP
