#ifndef CPROCESSWorkflowTESTS_H
#define CPROCESSWorkflowTESTS_H

#include <QObject>
#include "DataProcess.hpp"
#include "Core/CWorkflow.h"

class CWorkflowTests : public QObject
{
    Q_OBJECT

    public:

        CWorkflowTests(QObject *parent = nullptr);

    private slots:

        void            WorkflowConstructors();
        void            WorkflowSetters();
        void            WorkflowInputs();
        void            WorkflowTaskConnection();
        void            WorkflowStructure();

        void            buildSimpleWorkflow();
        void            buildSingleLineWorkflow();
        void            buildTwoLinesWorkflow();
        void            buildNestedWorkflows();

    private:

        WorkflowTaskPtr createTask(IODataType inputType, IODataType outputType);

        void            checkConnection(const CWorkflow &Workflow, const WorkflowEdge& e,
                                        const WorkflowVertex& src, size_t srcIndex,
                                        const WorkflowVertex& target, size_t targetIndex);

        CMat            loadSampleImage();

        void            showImage(const std::string &title, const CMat& img, bool bBlocking=false);

    private:

        CProcessRegistration m_processRegister;
};

#endif // CPROCESSWorkflowTESTS_H
