#ifndef CDNNTRAINPROCESSPARAMWRAP_H
#define CDNNTRAINPROCESSPARAMWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CDnnTrainProcess.h"

class CDnnTrainProcessParamWrap: public CDnnTrainProcessParam, public wrapper<CDnnTrainProcessParam>
{
    public:

        CDnnTrainProcessParamWrap();
        CDnnTrainProcessParamWrap(const CDnnTrainProcessParam& param);

        virtual void        setParamMap(const UMapString &paramMap) override;
        void                default_setParamMap(const UMapString& paramMap);

        virtual UMapString  getParamMap() const override;
        UMapString          default_getParamMap() const;

        virtual uint        getHashValue() const override;
        uint                default_getHashValue() const;
};

#endif // CDNNTRAINPROCESSPARAMWRAP_H
