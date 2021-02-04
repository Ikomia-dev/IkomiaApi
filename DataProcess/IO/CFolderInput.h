#ifndef CFOLDERINPUT_H
#define CFOLDERINPUT_H

#include "DataProcessGlobal.hpp"
#include "Protocol/CProtocolTaskIO.h"

class DATAPROCESSSHARED_EXPORT CFolderInput : public CProtocolTaskIO
{
    public:

        CFolderInput();
        CFolderInput(const std::string& path);

        void        setPath(const std::string& path);

        std::string getPath() const;

        bool        isDataAvailable() const override;

        void        clearData() override;

        std::shared_ptr<CProtocolTaskIO>    clone() const;

    private:

         std::shared_ptr<CProtocolTaskIO>   cloneImp() const override;

    private:

         std::string m_path = "";
};

class DATAPROCESSSHARED_EXPORT CFolderInputFactory: public CProtocolTaskIOFactory
{
    public:

        CFolderInputFactory()
        {
            m_name = "CFolderInput";
        }

        virtual ProtocolTaskIOPtr   create(IODataType dataType)
        {
            Q_UNUSED(dataType);
            return std::make_shared<CFolderInput>();
        }
};

#endif // CFOLDERINPUT_H
