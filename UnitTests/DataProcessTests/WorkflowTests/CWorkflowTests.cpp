#include <QString>
#include <QtTest>
#include "CWorkflowTests.h"
#include "UnitTestUtils.hpp"
#include "Protocol/CProtocolTaskIO.h"

CWorkflowTests::CWorkflowTests(QObject *parent) : QObject(parent)
{
}

void CWorkflowTests::protocolConstructors()
{
    CProtocol protocol1;
    QVERIFY(protocol1.getName().empty());
    QVERIFY(protocol1.getTaskCount() == 1);
    QVERIFY(protocol1.getDescription().empty());
    QVERIFY(protocol1.getKeywords().empty());
    QVERIFY(protocol1.getLastTaskId() == protocol1.getRootId());
    QVERIFY(protocol1.getActiveTaskId() == protocol1.getRootId());
    QVERIFY(protocol1.getSignalRawPtr() != nullptr);

    CProtocol protocol2("Protocol2");
    QVERIFY(protocol2.getName() == "Protocol2");
    QVERIFY(protocol2.getTaskCount() == 1);
    QVERIFY(protocol2.getDescription().empty());
    QVERIFY(protocol2.getKeywords().empty());
    QVERIFY(protocol2.getLastTaskId() == protocol2.getRootId());
    QVERIFY(protocol2.getActiveTaskId() == protocol2.getRootId());
    QVERIFY(protocol2.getSignalRawPtr() != nullptr);

    CProtocol protocol3(protocol2);
    QVERIFY(protocol3.getName() == protocol2.getName());
    QVERIFY(protocol3.getTaskCount() == protocol2.getTaskCount());
    QVERIFY(protocol3.getDescription() == protocol2.getDescription());
    QVERIFY(protocol3.getKeywords() == protocol2.getKeywords());
    QVERIFY(protocol3.getRootId() == protocol2.getRootId());
    QVERIFY(protocol3.getLastTaskId() == protocol2.getLastTaskId());
    QVERIFY(protocol3.getActiveTaskId() == protocol2.getActiveTaskId());
    QVERIFY(protocol3.getSignalRawPtr() != protocol2.getSignalRawPtr());

    CProtocol protocol4;
    protocol4 = protocol2;
    QVERIFY(protocol4.getName() == protocol2.getName());
    QVERIFY(protocol4.getTaskCount() == protocol2.getTaskCount());
    QVERIFY(protocol4.getDescription() == protocol2.getDescription());
    QVERIFY(protocol4.getKeywords() == protocol2.getKeywords());
    QVERIFY(protocol4.getRootId() == protocol2.getRootId());
    QVERIFY(protocol4.getLastTaskId() == protocol2.getLastTaskId());
    QVERIFY(protocol4.getActiveTaskId() == protocol2.getActiveTaskId());
    QVERIFY(protocol4.getSignalRawPtr() != protocol2.getSignalRawPtr());
}

void CWorkflowTests::protocolSetters()
{
    CProtocol protocol("MyProtocol");

    //Description
    std::string description = "This is my protocol";
    protocol.setDescription(description);
    QVERIFY(protocol.getDescription() == description);

    //Keywords
    std::string keywords = "Key1, key2, _key3, 1234, ??key";
    protocol.setKeywords(keywords);
    QVERIFY(protocol.getKeywords() == keywords);

    //Current task
    auto taskId = protocol.addTask(std::make_shared<CProtocolTask>());
    QVERIFY(protocol.getActiveTaskId() != taskId);
    protocol.setActiveTask(taskId);
    QVERIFY(protocol.getActiveTaskId() == taskId);
    ProtocolVertex invalidId = boost::graph_traits<ProtocolGraph>::null_vertex();
    protocol.setActiveTask(invalidId);
    QVERIFY(protocol.getActiveTaskId() != invalidId);
    QVERIFY(protocol.getActiveTaskId() == protocol.getRootId());
}

void CWorkflowTests::protocolInputs()
{
    CProtocol protocol("MyProtocol");
    auto pRootTask = protocol.getTask(protocol.getRootId());

    protocol.setInputs(InputOutputVect(), true);
    QVERIFY(protocol.getInputCount() == 0);
    QVERIFY(pRootTask->getInputCount() == 0);

    InputOutputVect defaultInputs;
    defaultInputs.push_back(std::make_shared<CProtocolTaskIO>());
    defaultInputs.push_back(std::make_shared<CProtocolTaskIO>());

    protocol.setInputs(defaultInputs, true);
    QVERIFY(protocol.getInputCount() == 2);
    QVERIFY(pRootTask->getInputCount() == 2);
    QVERIFY(protocol.getValidInputCount() == 2);
    QVERIFY(pRootTask->getValidInputCount() == 2);

    protocol.clearInputs();
    QVERIFY(protocol.getInputCount() == 0);
    QVERIFY(pRootTask->getInputCount() == 0);
    QVERIFY(protocol.getValidInputCount() == 0);
    QVERIFY(pRootTask->getValidInputCount() == 0);

    protocol.setInput(std::make_shared<CProtocolTaskIO>(), 2, true);
    QVERIFY(protocol.getInputCount() == 3);
    QVERIFY(pRootTask->getInputCount() == 3);
    QVERIFY(protocol.getValidInputCount() == 1);
    QVERIFY(pRootTask->getValidInputCount() == 1);


    protocol.setInput(std::make_shared<CProtocolTaskIO>(), 0, true);
    QVERIFY(protocol.getInputCount() == 3);
    QVERIFY(pRootTask->getInputCount() == 3);
    QVERIFY(protocol.getValidInputCount() == 2);
    QVERIFY(pRootTask->getValidInputCount() == 2);

    protocol.addInput(std::make_shared<CProtocolTaskIO>());
    QVERIFY(protocol.getInputCount() == 4);
    QVERIFY(pRootTask->getInputCount() == 4);
    QVERIFY(protocol.getValidInputCount() == 3);
    QVERIFY(pRootTask->getValidInputCount() == 3);

    protocol.addInputs(InputOutputVect());
    QVERIFY(protocol.getInputCount() == 4);
    QVERIFY(pRootTask->getInputCount() == 4);
    QVERIFY(protocol.getValidInputCount() == 3);
    QVERIFY(pRootTask->getValidInputCount() == 3);

    protocol.addInputs(defaultInputs);
    QVERIFY(protocol.getInputCount() == 6);
    QVERIFY(pRootTask->getInputCount() == 6);
    QVERIFY(protocol.getValidInputCount() == 5);
    QVERIFY(pRootTask->getValidInputCount() == 5);

    protocol.clearInputs();
    QVERIFY(protocol.getInputCount() == 0);
    QVERIFY(pRootTask->getInputCount() == 0);
    QVERIFY(protocol.getValidInputCount() == 0);
    QVERIFY(pRootTask->getValidInputCount() == 0);
}

void CWorkflowTests::protocolTaskConnection()
{
    CProtocol protocol("MyProtocol");
    auto rootId = protocol.getRootId();
    auto pRootTask = protocol.getTask(rootId);
    pRootTask->addOutput(std::make_shared<CProtocolTaskIO>(IODataType::IMAGE));

    auto pTask1 = std::make_shared<CProtocolTask>();
    pTask1->addInput(std::make_shared<CProtocolTaskIO>(IODataType::IMAGE));
    pTask1->addOutput(std::make_shared<CProtocolTaskIO>(IODataType::IMAGE));
    auto taskId1 = protocol.addTask(pTask1);

    //Connect root(0) -> task1(1) : FAILED -> input #1 for task1 does not exist
    QVERIFY_EXCEPTION_THROWN(protocol.connect(protocol.getRootId(), 0, taskId1, 1), CException);

    //Connect root(1) -> task1(0) : FAILED -> output #1 for root does not exist
    QVERIFY_EXCEPTION_THROWN(protocol.connect(protocol.getRootId(), 0, taskId1, 1), CException);

    //Connect task1(0) -> task1(0) : FAILED -> loop forbidden
    QVERIFY_EXCEPTION_THROWN(protocol.connect(taskId1, 0, taskId1, 0), CException);

    //Connect root(0) -> task1(0) : OK
    auto edgeId1 = protocol.connect(protocol.getRootId(), 0, taskId1, 0);
    checkConnection(protocol, edgeId1, protocol.getRootId(), 0, taskId1, 0);

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
            auto srcTaskId = protocol.addTask(pSrcTask);
            auto targetTaskId = protocol.addTask(pTargetTask);
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
                (dataSrc == IODataType::FOLDER && dataTarget == IODataType::IMAGE) ||
                (dataSrc == IODataType::FOLDER && dataTarget == IODataType::IMAGE_BINARY) ||
                (dataSrc == IODataType::FOLDER && dataTarget == IODataType::IMAGE_LABEL) ||
                (dataSrc == IODataType::FOLDER && dataTarget == IODataType::VIDEO) ||
                (dataSrc == IODataType::FOLDER && dataTarget == IODataType::VIDEO_BINARY) ||
                (dataSrc == IODataType::FOLDER && dataTarget == IODataType::VIDEO_LABEL) ||
                (dataSrc == IODataType::FOLDER && dataTarget == IODataType::FOLDER_PATH))
            {
                //Connect tasks: OK
                auto edgeId = protocol.connect(srcTaskId, 0, targetTaskId, 0);
                checkConnection(protocol, edgeId, srcTaskId, 0, targetTaskId, 0);
            }
            else
                QVERIFY_EXCEPTION_THROWN(protocol.connect(srcTaskId, 0, targetTaskId, 0), CException);
        }
    }
}

void CWorkflowTests::protocolStructure()
{
    CProtocol protocol("MyProtocol");
    QVERIFY(protocol.isRoot(protocol.getRootId()) == true);

    auto pTask1 = std::make_shared<CProtocolTask>("Task1");
    pTask1->addInput(std::make_shared<CProtocolTaskIO>(IODataType::IMAGE));
    pTask1->addOutput(std::make_shared<CProtocolTaskIO>(IODataType::IMAGE));
    auto taskId1 = protocol.addTask(pTask1);
    QVERIFY(protocol.isRoot(taskId1) == false);
    QVERIFY(protocol.getLastTaskId() == taskId1);

    auto pTask2 = std::make_shared<CProtocolTask>("Task2");
    pTask2->addInput(std::make_shared<CProtocolTaskIO>(IODataType::IMAGE));
    pTask2->addOutput(std::make_shared<CProtocolTaskIO>(IODataType::IMAGE));
    auto taskId2 = protocol.addTask(pTask1);
    QVERIFY(protocol.getLastTaskId() == taskId2);

    auto edge1 = protocol.connect(protocol.getRootId(), 0, taskId1, 0);
    Q_UNUSED(edge1);
    auto edge2 = protocol.connect(taskId1, 0, taskId2, 0);

    auto pTask3 = std::make_shared<CProtocolTask>("Task3");
    pTask3->addInput(std::make_shared<CProtocolTaskIO>(IODataType::IMAGE));
    pTask3->addOutput(std::make_shared<CProtocolTaskIO>(IODataType::IMAGE));

    protocol.replaceTask(pTask3, taskId1);
    auto pTaskTmp = protocol.getTask(taskId1);
    QVERIFY(protocol.getTaskCount() == 3);
    QVERIFY(pTaskTmp->getName() == "Task3");
    QVERIFY(protocol.getParents(taskId1).size() == 1);
    QVERIFY(protocol.getChilds(taskId1).size() == 1);

    protocol.deleteEdge(edge2);
    QVERIFY(protocol.getParents(taskId1).size() == 1);
    QVERIFY(protocol.getChilds(taskId1).size() == 0);

    protocol.deleteTask(taskId2);
    QVERIFY(protocol.getTaskCount() == 2);

    protocol.clear();
    QVERIFY(protocol.getTaskCount() == 0);
}

void CWorkflowTests::buildSimpleProtocol()
{
    CProtocol protocol("Simple Protocol");
    auto nullVertex = boost::graph_traits<ProtocolGraph>::null_vertex();
    auto factory = m_processRegister.getProcessFactory();

    //Add bilateral filter
    std::string name = tr("Bilateral Filter").toStdString();
    auto pTaskParam = std::make_shared<COcvBilateralParam>();
    auto pBilateral = factory.createObject(name, pTaskParam);
    QVERIFY(pBilateral != nullptr);
    auto vertexId = protocol.addTask(pBilateral);
    protocol.connect(nullVertex, 0, vertexId, 0);
    QVERIFY(vertexId != nullVertex);
    QVERIFY(protocol.getTaskCount() == 2);
    auto pTask = protocol[vertexId];
    QVERIFY(pTask == pBilateral);

    //Add image input
    auto pInput = std::make_shared<CImageProcessIO>();
    QVERIFY(pInput != nullptr);
    pInput->setImage(loadSampleImage());
    QVERIFY(pInput->isDataAvailable());
    //showImage("Protocol input", pInput->m_image);
    protocol.addInput(pInput);

    try
    {
        protocol.run();
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    //Get output
    auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(protocol.getOutput(0));
    QVERIFY(pOutput != nullptr);
    QVERIFY(pOutput->isDataAvailable());
        //showImage("Protocol output", pOutput->m_image, true);
}

void CWorkflowTests::buildSingleLineProtocol()
{
    CProtocol protocol("Single line Protocol");
    auto nullVertex = boost::graph_traits<ProtocolGraph>::null_vertex();
    auto factory = m_processRegister.getProcessFactory();

    //Add bilateral filter
    std::string name = tr("Bilateral Filter").toStdString();
    auto pBilateralParam = std::make_shared<COcvBilateralParam>();
    auto pBilateral = factory.createObject(name, pBilateralParam);
    auto bilateraId = protocol.addTask(pBilateral);
    protocol.connect(nullVertex, 0, bilateraId, 0);

    //Add box filter
    name = tr("Box Filter").toStdString();
    auto pBoxFilterParam = std::make_shared<COcvBoxFilterParam>();
    auto pBoxFilter = factory.createObject(name, pBoxFilterParam);
    auto boxFilterId = protocol.addTask(pBoxFilter);
    protocol.connect(bilateraId, 0, boxFilterId, 0);

    //Add Detail enhance filter
    name = tr("Detail Enhance Filter").toStdString();
    auto pDetailEnhanceParam = std::make_shared<COcvDetailEnhanceParam>();
    auto pDetailEnhance = factory.createObject(name, pDetailEnhanceParam);
    auto detailEnhanceId = protocol.addTask(pDetailEnhance);
    protocol.connect(boxFilterId, 0, detailEnhanceId, 0);

    //Add gray conversion filter
    name = tr("Color Conversion").toStdString();
    auto pCvtColorParam = std::make_shared<COcvCvtColorParam>();
    auto pCvtColor = factory.createObject(name, pCvtColorParam);
    auto cvtColorId = protocol.addTask(pCvtColor);
    protocol.connect(detailEnhanceId, 0, cvtColorId, 0);

    //Add cascade classifier
    name = tr("Cascade Classifier Filter").toStdString();
    std::string modelFile = "/usr/share/opencv/haarcascades/haarcascade_eye.xml";
    //std::string modelFile = "/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml";
    //std::string modelFile = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml";
    auto pCascadeClassifierParam = std::make_shared<COcvCascadeClassifierParam>(modelFile);
    auto pCascadeClassifier = factory.createObject(name, pCascadeClassifierParam);
    auto cascadeClassifierId = protocol.addTask(pCascadeClassifier);
    protocol.connect(cvtColorId, 0, cascadeClassifierId, 0);

    //Add image input
    auto pInput = std::make_shared<CImageProcessIO>();
    QVERIFY(pInput != nullptr);
    pInput->setImage(loadSampleImage());
    QVERIFY(pInput->isDataAvailable());
    //showImage("Protocol input", pInput->m_image);
    protocol.addInput(pInput);

    try
    {
        protocol.run();
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    //Get output
    auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(protocol.getOutput(0));
    QVERIFY(pOutput != nullptr);
    QVERIFY(pOutput->isDataAvailable());
}

void CWorkflowTests::buildTwoLinesProtocol()
{
    CProtocol protocol("Two lines Protocol");
    auto nullVertex = boost::graph_traits<ProtocolGraph>::null_vertex();
    auto factory = m_processRegister.getProcessFactory();

    //---------- First line ----------//
    //Add bilateral filter
    std::string name = tr("Bilateral Filter").toStdString();
    auto pBilateralParam = std::make_shared<COcvBilateralParam>();
    auto pBilateral = factory.createObject(name, pBilateralParam);
    auto bilateraId = protocol.addTask(pBilateral);
    protocol.connect(nullVertex, 0, bilateraId, 0);

    //Add box filter
    name = tr("Box Filter").toStdString();
    auto pBoxFilterParam = std::make_shared<COcvBoxFilterParam>();
    auto pBoxFilter = factory.createObject(name, pBoxFilterParam);
    auto boxFilterId = protocol.addTask(pBoxFilter);
    protocol.connect(bilateraId, 0, boxFilterId, 0);

    //---------- Second line ----------//
    //Add Adaptive manifold filter
    name = tr("Adaptive Manifold Filter").toStdString();
    auto pAdaptiveManifoldParam = std::make_shared<COcvAdaptiveManifoldParam>();
    auto pAdaptiveManifold = factory.createObject(name, pAdaptiveManifoldParam);
    auto adaptiveManifoldId = protocol.addTask(pAdaptiveManifold);
    protocol.connect(nullVertex, 0, adaptiveManifoldId, 0);

    //Add Detail enhance filter
    name = tr("Detail Enhance Filter").toStdString();
    auto pDetailEnhanceParam = std::make_shared<COcvDetailEnhanceParam>();
    auto pDetailEnhance = factory.createObject(name, pDetailEnhanceParam);
    auto detailEnhanceId = protocol.addTask(pDetailEnhance);
    protocol.connect(adaptiveManifoldId, 0, detailEnhanceId, 0);

    //---------- Join lines ----------//
    //Add Add weighted operation
    name = tr("Add Weighted").toStdString();
    auto pAddWeightedParam = std::make_shared<COcvAddWeightedParam>();
    auto pAddWeighted = factory.createObject(name, pAddWeightedParam);
    auto addWeightedId = protocol.addTask(pAddWeighted);
    protocol.connect(boxFilterId, 0, addWeightedId, 0);
    protocol.connect(detailEnhanceId, 0, addWeightedId, 1);

    //Add image input
    auto pInput = std::make_shared<CImageProcessIO>();
    pInput->setImage(loadSampleImage());
    //showImage("Protocol input", pInput->m_image);
    protocol.addInput(pInput);

    try
    {
        protocol.run();
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    //Get output
    /*auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(protocol.getOutput(0));
    if(pOutput)
        showImage("Protocol output", pOutput->m_image, true);*/
}

void CWorkflowTests::buildNestedProtocols()
{
    auto nullVertex = boost::graph_traits<ProtocolGraph>::null_vertex();
    auto factory = m_processRegister.getProcessFactory();

    try
    {
        //---------- Nested protocol ----------//
        auto pNestedProtocol = std::make_shared<CProtocol>("NestedProtocol");
        pNestedProtocol->setInput(std::make_shared<CImageProcessIO>(), 0, true);
        //Add Detail enhance filter
        std::string name = tr("Detail Enhance Filter").toStdString();
        auto pDetailEnhanceParam = std::make_shared<COcvDetailEnhanceParam>();
        auto pDetailEnhance = factory.createObject(name, pDetailEnhanceParam);
        auto detailEnhanceId = pNestedProtocol->addTask(pDetailEnhance);
        pNestedProtocol->connect(nullVertex, 0, detailEnhanceId, 0);

        //Add gray conversion filter
        name = tr("Color Conversion").toStdString();
        auto pCvtColorParam = std::make_shared<COcvCvtColorParam>();
        auto pCvtColor = factory.createObject(name, pCvtColorParam);
        auto cvtColorId = pNestedProtocol->addTask(pCvtColor);
        pNestedProtocol->connect(detailEnhanceId, 0, cvtColorId, 0);
        pNestedProtocol->addOutput(pCvtColor->getOutput(0));

        //---------- Main protocol ----------//
        CProtocol mainProtocol("MainProtocol");
        //Add bilateral filter
        name = tr("Bilateral Filter").toStdString();
        auto pBilateralParam = std::make_shared<COcvBilateralParam>();
        auto pBilateral = factory.createObject(name, pBilateralParam);
        auto bilateraId = mainProtocol.addTask(pBilateral);
        mainProtocol.connect(nullVertex, 0, bilateraId, 0);

        //Add nested protocol
        auto nestedProtocolId = mainProtocol.addTask(pNestedProtocol);
        mainProtocol.connect(bilateraId, 0, nestedProtocolId, 0);

        //Add cascade classifier
        name = tr("Cascade Classifier Filter").toStdString();
        std::string modelFile = "/usr/local/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml";
        auto pCascadeClassifierParam = std::make_shared<COcvCascadeClassifierParam>(modelFile);
        auto pCascadeClassifier = factory.createObject(name, pCascadeClassifierParam);
        auto cascadeClassifierId = mainProtocol.addTask(pCascadeClassifier);
        mainProtocol.connect(nestedProtocolId, 0, cascadeClassifierId, 0);

        //Add image input
        auto pInput = std::make_shared<CImageProcessIO>();
        pInput->setImage(loadSampleImage());
        //showImage("Protocol input", pInput->m_image);
        mainProtocol.addInput(pInput);

        //Run protocol
        mainProtocol.run();

        //Get output
        auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(mainProtocol.getOutput(0));
        QVERIFY(pOutput != nullptr);
        QVERIFY(pOutput->isDataAvailable());
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }
}

ProtocolTaskPtr CWorkflowTests::createTask(IODataType inputType, IODataType outputType)
{
    auto pTask = std::make_shared<CProtocolTask>();
    pTask->addInput(std::make_shared<CProtocolTaskIO>(inputType));
    pTask->addOutput(std::make_shared<CProtocolTaskIO>(outputType));
    return pTask;
}

void CWorkflowTests::checkConnection(const CProtocol &protocol, const ProtocolEdge &e, const ProtocolVertex &src, size_t srcIndex, const ProtocolVertex &target, size_t targetIndex)
{
    bool bFindParent = false;
    auto parents = protocol.getParents(target);

    for(size_t i=0; i<parents.size() && bFindParent == false; ++i)
    {
        if(parents[i] == src)
            bFindParent = true;
    }
    QVERIFY(bFindParent);

    bool bFindChild = false;
    auto childs = protocol.getChilds(src);

    for(size_t i=0; i<childs.size() && bFindChild == false; ++i)
    {
        if(childs[i] == target)
            bFindChild = true;
    }
    QVERIFY(bFindChild);

    auto pEdge = protocol.getEdge(e);
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
