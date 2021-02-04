#ifndef CGRAPHICSINPUTWRAP_H
#define CGRAPHICSINPUTWRAP_H

#include "PyDataProcessGlobal.h"
#include "IO/CGraphicsProcessInput.h"

class CGraphicsInputWrap : public CGraphicsProcessInput, public wrapper<CGraphicsProcessInput>
{
    public:

        CGraphicsInputWrap();
        CGraphicsInputWrap(const CGraphicsProcessInput &io);

        virtual bool    isDataAvailable() const;
        bool            default_isDataAvailable() const;

        virtual void    clearData();
        void            default_clearData();
};

#endif // CGRAPHICSINPUTWRAP_H
