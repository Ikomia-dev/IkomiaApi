#ifndef CPATHIO_H
#define CPATHIO_H

#include "DataProcessGlobal.hpp"
#include "Protocol/CProtocolTaskIO.h"

class CFolderInput;

class DATAPROCESSSHARED_EXPORT CPathIO : public CProtocolTaskIO
{
    public:

        CPathIO();
        CPathIO(IODataType data);
        CPathIO(IODataType data, const std::string& path);
        CPathIO(const CFolderInput& in);

        virtual ~CPathIO() = default;

        void        setPath(const std::string& path);

        std::string getPath() const;

        bool        isDataAvailable() const override;
        bool        isAutoInput() const override;

        void        clearData() override;

        void        copy(const std::shared_ptr<CProtocolTaskIO> &ioPtr) override;

        std::shared_ptr<CProtocolTaskIO>            clone() const;

    private:

        virtual std::shared_ptr<CProtocolTaskIO>    cloneImp() const override;

    private:

        std::string m_path;
};

class DATAPROCESSSHARED_EXPORT CPathIOFactory: public CProtocolTaskIOFactory
{
    public:

        CPathIOFactory()
        {
            m_name = "CPathIO";
        }

        virtual ProtocolTaskIOPtr   create(IODataType dataType)
        {
            return std::make_shared<CPathIO>(dataType);
        }
};

#endif // CPATHIO_H
