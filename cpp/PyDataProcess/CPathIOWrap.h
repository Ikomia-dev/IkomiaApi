#ifndef CPATHIOWRAP_H
#define CPATHIOWRAP_H

#include "PyDataProcessGlobal.h"
#include "IO/CPathIO.h"

class CPathIOWrap : public CPathIO, public wrapper<CPathIO>
{
    public:

        CPathIOWrap();
        CPathIOWrap(IODataType dataType);
        CPathIOWrap(IODataType dataType, const std::string& path);
        CPathIOWrap(IODataType dataType, const std::string& path, const std::string& name);
        CPathIOWrap(const CPathIO& io);

        virtual bool    isDataAvailable() const override;
        bool            default_isDataAvailable() const;

        virtual void    clearData() override;
        void            default_clearData();
};

#endif // CPATHIOWRAP_H
