#include <QString>
#include <QtTest>
#include "CWorkflowTests.h"
#include "UnitTestUtils.hpp"
#include "Workflow/CWorkflowTaskIO.h"

CWorkflowTests::CWorkflowTests(QObject *parent) : QObject(parent)
{
}

void CWorkflowTests::WorkflowConstructors()
{
    CWorkflow Workflow1;
    QVERIFY(Workflow1.getName().empty());
    QVERIFY(Workflow1.getTaskCount() == 1);
    QVERIFY(Workflow1.getDescription().empty());
    QVERIFY(Workflow1.getKeywords().empty());
    QVERIFY(Workflow1.getLastTaskId() == Workflow1.getRootId());
    QVERIFY(Workflow1.getActiveTaskId() == Workflow1.getRootId());
    QVERIFY(Workflow1.getSignalRawPtr() != nullptr);

    CWorkflow Workflow2("Workflow2");
    QVERIFY(Workflow2.getName() == "Workflow2");
    QVERIFY(Workflow2.getTaskCount() == 1);
    QVERIFY(Workflow2.getDescription().empty());
    QVERIFY(Workflow2.getKeywords().empty());
    QVERIFY(Workflow2.getLastTaskId() == Workflow2.getRootId());
    QVERIFY(Workflow2.getActiveTaskId() == Workflow2.getRootId());
    QVERIFY(Workflow2.getSignalRawPtr() != nullptr);

    CWorkflow Workflow3(Workflow2);
    QVERIFY(Workflow3.getName() == Workflow2.getName());
    QVERIFY(Workflow3.getTaskCount() == Workflow2.getTaskCount());
    QVERIFY(Workflow3.getDescription() == Workflow2.getDescription());
    QVERIFY(Workflow3.getKeywords() == Workflow2.getKeywords());
    QVERIFY(Workflow3.getRootId() == Workflow2.getRootId());
    QVERIFY(Workflow3.getLastTaskId() == Workflow2.getLastTaskId());
    QVERIFY(Workflow3.getActiveTaskId() == Workflow2.getActiveTaskId());
    QVERIFY(Workflow3.getSignalRawPtr() != Workflow2.getSignalRawPtr());

    CWorkflow Workflow4;
    Workflow4 = Workflow2;
    QVERIFY(Workflow4.getName() == Workflow2.getName());
    QVERIFY(Workflow4.getTaskCount() == Workflow2.getTaskCount());
    QVERIFY(Workflow4.getDescription() == Workflow2.getDescription());
    QVERIFY(Workflow4.getKeywords() == Workflow2.getKeywords());
    QVERIFY(Workflow4.getRootId() == Workflow2.getRootId());
    QVERIFY(Workflow4.getLastTaskId() == Workflow2.getLastTaskId());
    QVERIFY(Workflow4.getActiveTaskId() == Workflow2.getActiveTaskId());
    QVERIFY(Workflow4.getSignalRawPtr() != Workflow2.getSignalRawPtr());
}

void CWorkflowTests::WorkflowSetters()
{
    CWorkflow Workflow("MyWorkflow");

    //Description
    std::string description = "This is my Workflow";
    Workflow.setDescription(description);
    QVERIFY(Workflow.getDescription() == description);

    //Keywords
    std::string keywords = "Key1, key2, _key3, 1234, ??key";
    Workflow.setKeywords(keywords);
    QVERIFY(Workflow.getKeywords() == keywords);

    //Current task
    auto taskId = Workflow.addTask(std::make_shared<CWorkflowTask>());
    QVERIFY(Workflow.getActiveTaskId() != taskId);
    Workflow.setActiveTask(taskId);
    QVERIFY(Workflow.getActiveTaskId() == taskId);
    WorkflowVertex invalidId = boost::graph_traits<WorkflowGraph>::null_vertex();
    Workflow.setActiveTask(invalidId);
    QVERIFY(Workflow.getActiveTaskId() != invalidId);
    QVERIFY(Workflow.getActiveTaskId() == Workflow.getRootId());
}

void CWorkflowTests::WorkflowInputs()
{
    CWorkflow Workflow("MyWorkflow");
    auto pRootTask = Workflow.getTask(Workflow.getRootId());

    Workflow.setInputs(InputOutputVect(), true);
    QVERIFY(Workflow.getInputCount() == 0);
    QVERIFY(pRootTask->getInputCount() == 0);

    InputOutputVect defaultInputs;
    defaultInputs.push_back(std::make_shared<CWorkflowTaskIO>());
    defaultInputs.push_back(std::make_shared<CWorkflowTaskIO>());

    Workflow.setInputs(defaultInputs, true);
    QVERIFY(Workflow.getInputCount() == 2);
    QVERIFY(pRootTask->getInputCount() == 2);
    QVERIFY(Workflow.getValidInputCount() == 2);
    QVERIFY(pRootTask->getValidInputCount() == 2);

    Workflow.clearInputs();
    QVERIFY(Workflow.getInputCount() == 0);
    QVERIFY(pRootTask->getInputCount() == 0);
    QVERIFY(Workflow.getValidInputCount() == 0);
    QVERIFY(pRootTask->getValidInputCount() == 0);

    Workflow.setInput(std::make_shared<CWorkflowTaskIO>(), 2, true);
    QVERIFY(Workflow.getInputCount() == 3);
    QVERIFY(pRootTask->getInputCount() == 3);
    QVERIFY(Workflow.getValidInputCount() == 1);
    QVERIFY(pRootTask->getValidInputCount() == 1);


    Workflow.setInput(std::make_shared<CWorkflowTaskIO>(), 0, true);
    QVERIFY(Workflow.getInputCount() == 3);
    QVERIFY(pRootTask->getInputCount() == 3);
    QVERIFY(Workflow.getValidInputCount() == 2);
    QVERIFY(pRootTask->getValidInputCount() == 2);

    Workflow.addInput(std::make_shared<CWorkflowTaskIO>());
    QVERIFY(Workflow.getInputCount() == 4);
    QVERIFY(pRootTask->getInputCount() == 4);
    QVERIFY(Workflow.getValidInputCount() == 3);
    QVERIFY(pRootTask->getValidInputCount() == 3);

    Workflow.addInputs(InputOutputVect());
    QVERIFY(Workflow.getInputCount() == 4);
    QVERIFY(pRootTask->getInputCount() == 4);
    QVERIFY(Workflow.getValidInputCount() == 3);
    QVERIFY(pRootTask->getValidInputCount() == 3);

    Workflow.addInputs(defaultInputs);
    QVERIFY(Workflow.getInputCount() == 6);
    QVERIFY(pRootTask->getInputCount() == 6);
    QVERIFY(Workflow.getValidInputCount() == 5);
    QVERIFY(pRootTask->getValidInputCount() == 5);

    Workflow.clearInputs();
    QVERIFY(Workflow.getInputCount() == 0);
    QVERIFY(pRootTask->getInputCount() == 0);
    QVERIFY(Workflow.getValidInputCount() == 0);
    QVERIFY(pRootTask->getValidInputCount() == 0);
}

void CWorkflowTests::WorkflowTaskConnection()
{
    CWorkflow Workflow("MyWorkflow");
    auto rootId = Workflow.getRootId();
    auto pRootTask = Workflow.getTask(rootId);
    pRootTask->addOutput(std::make_shared<CWorkflowTaskIO>(IODataType::IMAGE));

    auto pTask1 = std::make_shared<CWorkflowTask>();
    pTask1->addInput(std::make_shared<CWorkflowTaskIO>(IODataType::IMAGE));
    pTask1->addOutput(std::make_shared<CWorkflowTaskIO>(IODataType::IMAGE));
    auto taskId1 = Workflow.addTask(pTask1);

    //Connect root(0) -> task1(1) : FAILED -> input #1 for task1 does not exist
    QVERIFY_EXCEPTION_THROWN(Workflow.connect(Workflow.getRootId(), 0, taskId1, 1), CException);

    //Connect root(1) -> task1(0) : FAILED -> output #1 for root does not exist
    QVERIFY_EXCEPTION_THROWN(Workflow.connect(Workflow.getRootId(), 0, taskId1, 1), CException);

    //Connect task1(0) -> task1(0) : FAILED -> loop forbidden
    QVERIFY_EXCEPTION_THROWN(Workflow.connect(taskId1, 0, taskId1, 0), CException);

    //Connect root(0) -> task1(0) : OK
    auto edgeId1 = Workflow.connect(Workflow.getRootId(), 0, taskId1, 0);
    checkConnection(Workflow, edgeId1, Workflow.getRootId(), 0, taskId1, 0);

    size_t firstType = static_cast<size_t>(IODataType::IMAGE);
    size_t lastType = static_cast<size_t>(IODataType::LIVE_STREAM_LABEL);

    //Test all cases of connection type
    for(size_t i=firstType; i<=lastType; ++i)
    {
        for(size_t j=firstType; j<=lastType; ++j)
        {
            //Create tasks
            auto pSrcTask = createTask(static_cast<IODataType>(i), static_cast<IODataType>(i));
            auto pTargetTask = createTask(static_cast<IODataType>(j), static_cast<IODataType>(j));
            auto srcTaskId = Workflow.addTask(pSrcTask);
            auto targetTaskId = Workflow.addTask(pTargetTask);
            IODataType dataSrc = static_cast<IODataType>(i);
            IODataType dataTarget = static_cast<IODataType>(j);

            if( (dataSrc == dataTarget) ||
                (dataSrc == IODataType::IMAGE_BINARY && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::IMAGE_BINARY && dataTarget == IODataType::IMAGE_LABEL) ||
                (dataSrc == IODataType::IMAGE_LABEL && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::VOLUME && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::VOLUME_BINARY && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::VOLUME_BINARY && dataTarget == IODataType::IMAGE_BINARY) ||
                (dataSrc == IODataType::VOLUME_BINARY && dataTarget == IODataType::IMAGE_LABEL) ||
                (dataSrc == IODataType::VOLUME_BINARY && dataTarget == IODataType::VOLUME) ||
                (dataSrc == IODataType::VOLUME_BINARY && dataTarget == IODataType::VOLUME_LABEL) ||
                (dataSrc == IODataType::VOLUME_LABEL && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::VOLUME_LABEL && dataTarget == IODataType::IMAGE_LABEL) ||
                (dataSrc == IODataType::VIDEO && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::VIDEO_BINARY && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::VIDEO_BINARY && dataTarget == IODataType::IMAGE_BINARY) ||
                (dataSrc == IODataType::VIDEO_BINARY && dataTarget == IODataType::IMAGE_LABEL) ||
                (dataSrc == IODataType::VIDEO_BINARY && dataTarget == IODataType::VIDEO) ||
                (dataSrc == IODataType::VIDEO_BINARY && dataTarget == IODataType::VIDEO_LABEL) ||
                (dataSrc == IODataType::VIDEO_LABEL && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::VIDEO_LABEL && dataTarget == IODataType::IMAGE_LABEL) ||
                (dataSrc == IODataType::VIDEO_LABEL && dataTarget == IODataType::VIDEO) ||
                (dataSrc == IODataType::LIVE_STREAM && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::LIVE_STREAM && dataTarget == IODataType::VIDEO) ||
                (dataSrc == IODataType::LIVE_STREAM_BINARY && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::LIVE_STREAM_BINARY && dataTarget == IODataType::IMAGE_BINARY) ||
                (dataSrc == IODataType::LIVE_STREAM_BINARY && dataTarget == IODataType::IMAGE_LABEL) ||
                (dataSrc == IODataType::LIVE_STREAM_BINARY && dataTarget == IODataType::VIDEO) ||
                (dataSrc == IODataType::LIVE_STREAM_BINARY && dataTarget == IODataType::VIDEO_BINARY) ||
                (dataSrc == IODataType::LIVE_STREAM_BINARY && dataTarget == IODataType::VIDEO_LABEL) ||
                (dataSrc == IODataType::LIVE_STREAM_BINARY && dataTarget == IODataType::LIVE_STREAM) ||
                (dataSrc == IODataType::LIVE_STREAM_BINARY && dataTarget == IODataType::LIVE_STREAM_LABEL) ||
                (dataSrc == IODataType::LIVE_STREAM_LABEL && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::LIVE_STREAM_LABEL && dataTarget == IODataType::IMAGE_LABEL) ||
                (dataSrc == IODataType::LIVE_STREAM_LABEL && dataTarget == IODataType::VIDEO) ||
                (dataSrc == IODataType::LIVE_STREAM_LABEL && dataTarget == IODataType::VIDEO_LABEL) ||
                (dataSrc == IODataType::LIVE_STREAM_LABEL && dataTarget == IODataType::LIVE_STREAM) ||
                (dataSrc == IODataType::INPUT_GRAPHICS && dataTarget == IODataType::OUTPUT_GRAPHICS) ||
                (dataSrc == IODataType::OUTPUT_GRAPHICS && dataTarget == IODataType::INPUT_GRAPHICS) ||
                (dataSrc == IODataType::PROJECT_FOLDER && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::PROJECT_FOLDER && dataTarget == IODataType::IMAGE_BINARY) ||
                (dataSrc == IODataType::PROJECT_FOLDER && dataTarget == IODataType::IMAGE_LABEL) ||
                (dataSrc == IODataType::PROJECT_FOLDER && dataTarget == IODataType::VIDEO) ||
                (dataSrc == IODataType::PROJECT_FOLDER && dataTarget == IODataType::VIDEO_BINARY) ||
                (dataSrc == IODataType::PROJECT_FOLDER && dataTarget == IODataType::VIDEO_LABEL) ||
                (dataSrc == IODataType::PROJECT_FOLDER && dataTarget == IODataType::FOLDER_PATH))
            {
                //Connect tasks: OK
                auto edgeId = Workflow.connect(srcTaskId, 0, targetTaskId, 0);
                checkConnection(Workflow, edgeId, srcTaskId, 0, targetTaskId, 0);
            }
            else
                QVERIFY_EXCEPTION_THROWN(Workflow.connect(srcTaskId, 0, targetTaskId, 0), CException);
        }
    }
}

void CWorkflowTests::WorkflowStructure()
{
    CWorkflow Workflow("MyWorkflow");
    QVERIFY(Workflow.isRoot(Workflow.getRootId()) == true);

    auto pTask1 = std::make_shared<CWorkflowTask>("Task1");
    pTask1->addInput(std::make_shared<CWorkflowTaskIO>(IODataType::IMAGE));
    pTask1->addOutput(std::make_shared<CWorkflowTaskIO>(IODataType::IMAGE));
    auto taskId1 = Workflow.addTask(pTask1);
    QVERIFY(Workflow.isRoot(taskId1) == false);
    QVERIFY(Workflow.getLastTaskId() == taskId1);

    auto pTask2 = std::make_shared<CWorkflowTask>("Task2");
    pTask2->addInput(std::make_shared<CWorkflowTaskIO>(IODataType::IMAGE));
    pTask2->addOutput(std::make_shared<CWorkflowTaskIO>(IODataType::IMAGE));
    auto taskId2 = Workflow.addTask(pTask1);
    QVERIFY(Workflow.getLastTaskId() == taskId2);

    auto edge1 = Workflow.connect(Workflow.getRootId(), 0, taskId1, 0);
    Q_UNUSED(edge1);
    auto edge2 = Workflow.connect(taskId1, 0, taskId2, 0);

    auto pTask3 = std::make_shared<CWorkflowTask>("Task3");
    pTask3->addInput(std::make_shared<CWorkflowTaskIO>(IODataType::IMAGE));
    pTask3->addOutput(std::make_shared<CWorkflowTaskIO>(IODataType::IMAGE));

    Workflow.replaceTask(pTask3, taskId1);
    auto pTaskTmp = Workflow.getTask(taskId1);
    QVERIFY(Workflow.getTaskCount() == 3);
    QVERIFY(pTaskTmp->getName() == "Task3");
    QVERIFY(Workflow.getParents(taskId1).size() == 1);
    QVERIFY(Workflow.getChilds(taskId1).size() == 1);

    Workflow.deleteEdge(edge2);
    QVERIFY(Workflow.getParents(taskId1).size() == 1);
    QVERIFY(Workflow.getChilds(taskId1).size() == 0);

    Workflow.deleteTask(taskId2);
    QVERIFY(Workflow.getTaskCount() == 2);

    Workflow.clear();
    QVERIFY(Workflow.getTaskCount() == 0);
}

void CWorkflowTests::buildSimpleWorkflow()
{
    CWorkflow Workflow("Simple Workflow");
    auto nullVertex = boost::graph_traits<WorkflowGraph>::null_vertex();
    auto factory = m_processRegister.getProcessFactory();

    //Add bilateral filter
    std::string name = tr("Bilateral Filter").toStdString();
    auto pTaskParam = std::make_shared<COcvBilateralParam>();
    auto pBilateral = factory.createObject(name, pTaskParam);
    QVERIFY(pBilateral != nullptr);
    auto vertexId = Workflow.addTask(pBilateral);
    Workflow.connect(nullVertex, 0, vertexId, 0);
    QVERIFY(vertexId != nullVertex);
    QVERIFY(Workflow.getTaskCount() == 2);
    auto pTask = Workflow[vertexId];
    QVERIFY(pTask == pBilateral);

    //Add image input
    auto pInput = std::make_shared<CImageIO>();
    QVERIFY(pInput != nullptr);
    pInput->setImage(loadSampleImage());
    QVERIFY(pInput->isDataAvailable());
    //showImage("Workflow input", pInput->m_image);
    Workflow.addInput(pInput);

    try
    {
        Workflow.run();
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    //Get output
    auto pOutput = std::dynamic_pointer_cast<CImageIO>(Workflow.getOutput(0));
    QVERIFY(pOutput != nullptr);
    QVERIFY(pOutput->isDataAvailable());
        //showImage("Workflow output", pOutput->m_image, true);
}

void CWorkflowTests::buildSingleLineWorkflow()
{
    CWorkflow Workflow("Single line Workflow");
    auto nullVertex = boost::graph_traits<WorkflowGraph>::null_vertex();
    auto factory = m_processRegister.getProcessFactory();

    //Add bilateral filter
    std::string name = tr("Bilateral Filter").toStdString();
    auto pBilateralParam = std::make_shared<COcvBilateralParam>();
    auto pBilateral = factory.createObject(name, pBilateralParam);
    auto bilateraId = Workflow.addTask(pBilateral);
    Workflow.connect(nullVertex, 0, bilateraId, 0);

    //Add box filter
    name = tr("Box Filter").toStdString();
    auto pBoxFilterParam = std::make_shared<COcvBoxFilterParam>();
    auto pBoxFilter = factory.createObject(name, pBoxFilterParam);
    auto boxFilterId = Workflow.addTask(pBoxFilter);
    Workflow.connect(bilateraId, 0, boxFilterId, 0);

    //Add Detail enhance filter
    name = tr("Detail Enhance Filter").toStdString();
    auto pDetailEnhanceParam = std::make_shared<COcvDetailEnhanceParam>();
    auto pDetailEnhance = factory.createObject(name, pDetailEnhanceParam);
    auto detailEnhanceId = Workflow.addTask(pDetailEnhance);
    Workflow.connect(boxFilterId, 0, detailEnhanceId, 0);

    //Add gray conversion filter
    name = tr("Color Conversion").toStdString();
    auto pCvtColorParam = std::make_shared<COcvCvtColorParam>();
    auto pCvtColor = factory.createObject(name, pCvtColorParam);
    auto cvtColorId = Workflow.addTask(pCvtColor);
    Workflow.connect(detailEnhanceId, 0, cvtColorId, 0);

    //Add cascade classifier
    name = tr("Cascade Classifier Filter").toStdString();
    std::string modelFile = "/usr/share/opencv/haarcascades/haarcascade_eye.xml";
    //std::string modelFile = "/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml";
    //std::string modelFile = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml";
    auto pCascadeClassifierParam = std::make_shared<COcvCascadeClassifierParam>(modelFile);
    auto pCascadeClassifier = factory.createObject(name, pCascadeClassifierParam);
    auto cascadeClassifierId = Workflow.addTask(pCascadeClassifier);
    Workflow.connect(cvtColorId, 0, cascadeClassifierId, 0);

    //Add image input
    auto pInput = std::make_shared<CImageIO>();
    QVERIFY(pInput != nullptr);
    pInput->setImage(loadSampleImage());
    QVERIFY(pInput->isDataAvailable());
    //showImage("Workflow input", pInput->m_image);
    Workflow.addInput(pInput);

    try
    {
        Workflow.run();
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    //Get output
    auto pOutput = std::dynamic_pointer_cast<CImageIO>(Workflow.getOutput(0));
    QVERIFY(pOutput != nullptr);
    QVERIFY(pOutput->isDataAvailable());
}

void CWorkflowTests::buildTwoLinesWorkflow()
{
    CWorkflow Workflow("Two lines Workflow");
    auto nullVertex = boost::graph_traits<WorkflowGraph>::null_vertex();
    auto factory = m_processRegister.getProcessFactory();

    //---------- First line ----------//
    //Add bilateral filter
    std::string name = tr("Bilateral Filter").toStdString();
    auto pBilateralParam = std::make_shared<COcvBilateralParam>();
    auto pBilateral = factory.createObject(name, pBilateralParam);
    auto bilateraId = Workflow.addTask(pBilateral);
    Workflow.connect(nullVertex, 0, bilateraId, 0);

    //Add box filter
    name = tr("Box Filter").toStdString();
    auto pBoxFilterParam = std::make_shared<COcvBoxFilterParam>();
    auto pBoxFilter = factory.createObject(name, pBoxFilterParam);
    auto boxFilterId = Workflow.addTask(pBoxFilter);
    Workflow.connect(bilateraId, 0, boxFilterId, 0);

    //---------- Second line ----------//
    //Add Adaptive manifold filter
    name = tr("Adaptive Manifold Filter").toStdString();
    auto pAdaptiveManifoldParam = std::make_shared<COcvAdaptiveManifoldParam>();
    auto pAdaptiveManifold = factory.createObject(name, pAdaptiveManifoldParam);
    auto adaptiveManifoldId = Workflow.addTask(pAdaptiveManifold);
    Workflow.connect(nullVertex, 0, adaptiveManifoldId, 0);

    //Add Detail enhance filter
    name = tr("Detail Enhance Filter").toStdString();
    auto pDetailEnhanceParam = std::make_shared<COcvDetailEnhanceParam>();
    auto pDetailEnhance = factory.createObject(name, pDetailEnhanceParam);
    auto detailEnhanceId = Workflow.addTask(pDetailEnhance);
    Workflow.connect(adaptiveManifoldId, 0, detailEnhanceId, 0);

    //---------- Join lines ----------//
    //Add Add weighted operation
    name = tr("Add Weighted").toStdString();
    auto pAddWeightedParam = std::make_shared<COcvAddWeightedParam>();
    auto pAddWeighted = factory.createObject(name, pAddWeightedParam);
    auto addWeightedId = Workflow.addTask(pAddWeighted);
    Workflow.connect(boxFilterId, 0, addWeightedId, 0);
    Workflow.connect(detailEnhanceId, 0, addWeightedId, 1);

    //Add image input
    auto pInput = std::make_shared<CImageIO>();
    pInput->setImage(loadSampleImage());
    //showImage("Workflow input", pInput->m_image);
    Workflow.addInput(pInput);

    try
    {
        Workflow.run();
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    //Get output
    /*auto pOutput = std::dynamic_pointer_cast<CImageIO>(Workflow.getOutput(0));
    if(pOutput)
        showImage("Workflow output", pOutput->m_image, true);*/
}

void CWorkflowTests::buildNestedWorkflows()
{
    auto nullVertex = boost::graph_traits<WorkflowGraph>::null_vertex();
    auto factory = m_processRegister.getProcessFactory();

    try
    {
        //---------- Nested Workflow ----------//
        auto pNestedWorkflow = std::make_shared<CWorkflow>("NestedWorkflow");
        pNestedWorkflow->setInput(std::make_shared<CImageIO>(), 0, true);
        //Add Detail enhance filter
        std::string name = tr("Detail Enhance Filter").toStdString();
        auto pDetailEnhanceParam = std::make_shared<COcvDetailEnhanceParam>();
        auto pDetailEnhance = factory.createObject(name, pDetailEnhanceParam);
        auto detailEnhanceId = pNestedWorkflow->addTask(pDetailEnhance);
        pNestedWorkflow->connect(nullVertex, 0, detailEnhanceId, 0);

        //Add gray conversion filter
        name = tr("Color Conversion").toStdString();
        auto pCvtColorParam = std::make_shared<COcvCvtColorParam>();
        auto pCvtColor = factory.createObject(name, pCvtColorParam);
        auto cvtColorId = pNestedWorkflow->addTask(pCvtColor);
        pNestedWorkflow->connect(detailEnhanceId, 0, cvtColorId, 0);
        pNestedWorkflow->addOutput(pCvtColor->getOutput(0));

        //---------- Main Workflow ----------//
        CWorkflow mainWorkflow("MainWorkflow");
        //Add bilateral filter
        name = tr("Bilateral Filter").toStdString();
        auto pBilateralParam = std::make_shared<COcvBilateralParam>();
        auto pBilateral = factory.createObject(name, pBilateralParam);
        auto bilateraId = mainWorkflow.addTask(pBilateral);
        mainWorkflow.connect(nullVertex, 0, bilateraId, 0);

        //Add nested Workflow
        auto nestedWorkflowId = mainWorkflow.addTask(pNestedWorkflow);
        mainWorkflow.connect(bilateraId, 0, nestedWorkflowId, 0);

        //Add cascade classifier
        name = tr("Cascade Classifier Filter").toStdString();
        std::string modelFile = "/usr/local/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml";
        auto pCascadeClassifierParam = std::make_shared<COcvCascadeClassifierParam>(modelFile);
        auto pCascadeClassifier = factory.createObject(name, pCascadeClassifierParam);
        auto cascadeClassifierId = mainWorkflow.addTask(pCascadeClassifier);
        mainWorkflow.connect(nestedWorkflowId, 0, cascadeClassifierId, 0);

        //Add image input
        auto pInput = std::make_shared<CImageIO>();
        pInput->setImage(loadSampleImage());
        //showImage("Workflow input", pInput->m_image);
        mainWorkflow.addInput(pInput);

        //Run Workflow
        mainWorkflow.run();

        //Get output
        auto pOutput = std::dynamic_pointer_cast<CImageIO>(mainWorkflow.getOutput(0));
        QVERIFY(pOutput != nullptr);
        QVERIFY(pOutput->isDataAvailable());
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }
}

WorkflowTaskPtr CWorkflowTests::createTask(IODataType inputType, IODataType outputType)
{
    auto pTask = std::make_shared<CWorkflowTask>();
    pTask->addInput(std::make_shared<CWorkflowTaskIO>(inputType));
    pTask->addOutput(std::make_shared<CWorkflowTaskIO>(outputType));
    return pTask;
}

void CWorkflowTests::checkConnection(const CWorkflow &Workflow, const WorkflowEdge &e, const WorkflowVertex &src, size_t srcIndex, const WorkflowVertex &target, size_t targetIndex)
{
    bool bFindParent = false;
    auto parents = Workflow.getParents(target);

    for(size_t i=0; i<parents.size() && bFindParent == false; ++i)
    {
        if(parents[i] == src)
            bFindParent = true;
    }
    QVERIFY(bFindParent);

    bool bFindChild = false;
    auto childs = Workflow.getChilds(src);

    for(size_t i=0; i<childs.size() && bFindChild == false; ++i)
    {
        if(childs[i] == target)
            bFindChild = true;
    }
    QVERIFY(bFindChild);

    auto pEdge = Workflow.getEdge(e);
    QVERIFY(pEdge != nullptr);
    QVERIFY(pEdge->getSourceIndex() == srcIndex);
    QVERIFY(pEdge->getTargetIndex() == targetIndex);
}

CMat CWorkflowTests::loadSampleImage()
{
    std::string imagePath = UnitTest::getDataPath() + "/Images/Lena.png";
    CMat img = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
    return img;
}

void CWorkflowTests::showImage(const std::string& title, const CMat &img, bool bBlocking)
{
    cv::imshow(title, img);
    if(bBlocking)
        cv::waitKey(0);
}

QTEST_GUILESS_MAIN(CWorkflowTests)
