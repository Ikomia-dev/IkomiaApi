#ifndef CPROCESSPROTOCOLTESTS_H
#define CPROCESSPROTOCOLTESTS_H

#include <QObject>
#include "DataProcess.hpp"
#include "Core/CProtocol.h"

class CWorkflowTests : public QObject
{
    Q_OBJECT

    public:

        CWorkflowTests(QObject *parent = nullptr);

    private slots:

        void            protocolConstructors();
        void            protocolSetters();
        void            protocolInputs();
        void            protocolTaskConnection();
        void            protocolStructure();

        void            buildSimpleProtocol();
        void            buildSingleLineProtocol();
        void            buildTwoLinesProtocol();
        void            buildNestedProtocols();

    private:

        ProtocolTaskPtr createTask(IODataType inputType, IODataType outputType);

        void            checkConnection(const CProtocol &protocol, const ProtocolEdge& e,
                                        const ProtocolVertex& src, size_t srcIndex,
                                        const ProtocolVertex& target, size_t targetIndex);

        CMat            loadSampleImage();

        void            showImage(const std::string &title, const CMat& img, bool bBlocking=false);

    private:

        CProcessRegistration m_processRegister;
};

#endif // CPROCESSPROTOCOLTESTS_H
