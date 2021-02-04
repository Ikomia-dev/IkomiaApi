#include "CDnnTrainProcess.h"
#include <QDesktopServices>
#include "UtilsTools.hpp"

//---------------------------------//
//----- CDnnTrainProcessParam -----//
//---------------------------------//
CDnnTrainProcessParam::CDnnTrainProcessParam() : CProtocolTaskParam()
{
}

void CDnnTrainProcessParam::setParamMap(const UMapString &paramMap)
{
    m_batchSize = std::stoi(paramMap.at("batchSize"));
    m_epochs = std::stoi(paramMap.at("epochs"));
    m_classes = std::stoi(paramMap.at("classes"));
    m_learningRate = std::stof(paramMap.at("learningRate"));
    m_momentum = std::stof(paramMap.at("momentum"));
    m_modelName = paramMap.at("modelName");
}

void CDnnTrainProcessParam::setBatchSize(int size)
{
    m_batchSize = size;
}

void CDnnTrainProcessParam::setEpochCount(int nb)
{
    m_epochs = nb;
}

void CDnnTrainProcessParam::setClassCount(int nb)
{
    m_classes = nb;
}

void CDnnTrainProcessParam::setLearningRate(float value)
{
    m_learningRate = value;
}

void CDnnTrainProcessParam::setMomentum(float value)
{
    m_momentum = value;
}

void CDnnTrainProcessParam::setWeightDecay(float decay)
{
    m_weightDecay = decay;
}

void CDnnTrainProcessParam::setModelName(const std::string &name)
{
    m_modelName = name;
}

UMapString CDnnTrainProcessParam::getParamMap() const
{
    UMapString map;
    map.insert(std::make_pair("batchSize", std::to_string(m_batchSize)));
    map.insert(std::make_pair("epochs", std::to_string(m_epochs)));
    map.insert(std::make_pair("classes", std::to_string(m_classes)));
    map.insert(std::make_pair("learningRate", std::to_string(m_learningRate)));
    map.insert(std::make_pair("momentum", std::to_string(m_momentum)));
    map.insert(std::make_pair("modelName", m_modelName));
    return map;
}

uint CDnnTrainProcessParam::getHashValue() const
{
    return CProtocolTaskParam::getHashValue();
}

int CDnnTrainProcessParam::getBatchSize() const
{
    return m_batchSize;
}

int CDnnTrainProcessParam::getEpochCount() const
{
    return m_epochs;
}

int CDnnTrainProcessParam::getClassCount() const
{
    return m_classes;
}

float CDnnTrainProcessParam::getLearningRate() const
{
    return m_learningRate;
}

float CDnnTrainProcessParam::getMomentum() const
{
    return m_momentum;
}

float CDnnTrainProcessParam::getWeightDecay() const
{
    return m_weightDecay;
}

std::string CDnnTrainProcessParam::getModelName() const
{
    return m_modelName;
}

//----------------------------//
//----- CDnnTrainProcess -----//
//----------------------------//
CDnnTrainProcess::CDnnTrainProcess() : CProtocolTask()
{
    m_type = CProtocolTask::Type::DNN_TRAIN;
    m_pParam = std::make_shared<CDnnTrainProcessParam>();
}

CDnnTrainProcess::CDnnTrainProcess(const std::string &name): CProtocolTask(name)
{
    m_type = CProtocolTask::Type::DNN_TRAIN;
    m_pParam = std::make_shared<CDnnTrainProcessParam>();
}

CDnnTrainProcess::CDnnTrainProcess(const std::string &name, const std::shared_ptr<CDnnTrainProcessParam> &pParam) : CProtocolTask(name)
{
    m_type = CProtocolTask::Type::DNN_TRAIN;

    if(pParam)
        m_pParam = std::make_shared<CDnnTrainProcessParam>(*pParam);
    else
        m_pParam = std::make_shared<CDnnTrainProcessParam>();
}

void CDnnTrainProcess::beginTaskRun()
{
    CProtocolTask::beginTaskRun();

    //Show MLflow server UI
    QDesktopServices::openUrl(QUrl(QString::fromStdString(Utils::MLflow::getTrackingURI())));
}

void CDnnTrainProcess::endTaskRun()
{
    CProtocolTask::endTaskRun();
}
