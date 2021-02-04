#ifndef CWIDGETOUTPUTWRAP_H
#define CWIDGETOUTPUTWRAP_H

#include "PyDataProcessGlobal.h"
#include "IO/CWidgetOutput.h"

class CWidgetOutputWrap : public CWidgetOutput, public wrapper<CWidgetOutput>
{
    public:

        CWidgetOutputWrap();
        CWidgetOutputWrap(IODataType dataType);
        CWidgetOutputWrap(const CWidgetOutput& out);

        void            setWidget(long long widgetPtr);

        virtual bool    isDataAvailable() const;
        bool            default_isDataAvailable() const;

        virtual void    clearData();
        void            default_clearData();
};

#endif // CWIDGETOUTPUTWRAP_H
