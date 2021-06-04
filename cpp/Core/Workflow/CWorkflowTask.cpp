// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <QJsonArray>
#include "CWorkflowTask.h"
#include "CException.h"
#include "Graphics/CGraphicsLayer.h"
#include "UtilsTools.hpp"
#include "Main/CoreTools.hpp"

//--------------------------------//
//----- CSignalHandler class -----//
//--------------------------------//
CSignalHandler::CSignalHandler()
{
}

void CSignalHandler::onAddGraphicsLayer(CGraphicsLayer *pLayer)
{
    emit doAddGraphicsLayer(pLayer);
}

void CSignalHandler::onRemoveGraphicsLayer(CGraphicsLayer *pLayer)
{
    emit doRemoveGraphicsLayer(pLayer);
}

void CSignalHandler::onFinishWorkflow()
{
    emit doFinishWorkflow();
}

void CSignalHandler::onOutputChanged()
{
    emit doOutputChanged();
}

void CSignalHandler::onGraphicsContextChanged()
{
    emit doGraphicsContextChanged();
}

//--------------------------------//
//----- CWorkflowTask class -----//
//--------------------------------//
CWorkflowTask::CWorkflowTask() : m_signalHandler(std::make_unique<CSignalHandler>())
{
}

CWorkflowTask::CWorkflowTask(const std::string &name) : m_signalHandler(std::make_unique<CSignalHandler>())
{
    m_name = name;
}

CWorkflowTask::CWorkflowTask(const CWorkflowTask &task) : m_signalHandler(std::make_unique<CSignalHandler>())
{
    m_name = task.m_name;
    m_outputFolder = task.m_outputFolder;
    m_inputs = task.m_inputs;
    m_originalInputTypes = task.m_originalInputTypes;
    m_inputViewProps = task.m_inputViewProps;
    m_outputs = task.m_outputs;
    m_outputViewProps = task.m_outputViewProps;
    m_actionFlags = task.m_actionFlags;
    m_bActive = task.m_bActive;

    if(task.m_pParam)
        m_pParam = std::make_shared<CWorkflowTaskParam>(*task.m_pParam);
    else
        m_pParam = nullptr;
}

CWorkflowTask::CWorkflowTask(const CWorkflowTask&& task) : m_signalHandler(std::make_unique<CSignalHandler>())
{
    m_name = std::move(task.m_name);
    m_outputFolder = std::move(task.m_outputFolder);
    m_inputs = std::move(task.m_inputs);
    m_originalInputTypes = std::move(task.m_originalInputTypes);
    m_inputViewProps = std::move(task.m_inputViewProps);
    m_outputs = std::move(task.m_outputs);
    m_outputViewProps = std::move(task.m_outputViewProps);
    m_actionFlags = std::move(task.m_actionFlags);
    m_bActive = std::move(task.m_bActive);

    if(task.m_pParam)
        m_pParam = std::make_shared<CWorkflowTaskParam>(std::move(*task.m_pParam));
    else
        m_pParam = nullptr;
}

CWorkflowTask &CWorkflowTask::operator=(const CWorkflowTask &task)
{
    m_signalHandler = std::make_unique<CSignalHandler>();
    m_name = task.m_name;
    m_outputFolder = task.m_outputFolder;
    m_inputs = task.m_inputs;
    m_originalInputTypes = task.m_originalInputTypes;
    m_inputViewProps = task.m_inputViewProps;
    m_outputs = task.m_outputs;
    m_outputViewProps = task.m_outputViewProps;
    m_actionFlags = task.m_actionFlags;
    m_pParam = task.m_pParam;
    m_bActive = task.m_bActive;
    return *this;
}

CWorkflowTask::~CWorkflowTask()
{
    CPyEnsureGIL gil;
    m_inputs.clear();
    m_outputs.clear();
}

CWorkflowTask &CWorkflowTask::operator=(const CWorkflowTask&& task)
{ 
    m_signalHandler = std::make_unique<CSignalHandler>();
    m_name = std::move(task.m_name);
    m_outputFolder = std::move(task.m_outputFolder);
    m_inputs = std::move(task.m_inputs);
    m_originalInputTypes = std::move(task.m_originalInputTypes);
    m_inputViewProps = std::move(task.m_inputViewProps);
    m_outputs = std::move(task.m_outputs);
    m_outputViewProps = std::move(task.m_outputViewProps);
    m_actionFlags = std::move(task.m_actionFlags);
    m_pParam = std::move(task.m_pParam);
    m_bActive = std::move(task.m_bActive);
    return *this;
}

void CWorkflowTask::setInputDataType(const IODataType &dataType, size_t index)
{
    if(index < m_inputs.size())
        m_inputs[index]->setDataType(dataType);
    else
    {
        m_inputs.resize(index + 1);
        m_inputs[index]->setDataType(dataType);
    }
}

void CWorkflowTask::setInput(const WorkflowTaskIOPtr &pInput, size_t index, bool bNewSequence)
{    
    Q_UNUSED(bNewSequence);        

    if(index >= m_inputs.size())
    {
        //New input
        m_inputs.resize(index + 1);
        m_originalInputTypes.resize(index + 1);
        m_inputViewProps.emplace_back(CViewPropertyIO());

        if(pInput)
            m_originalInputTypes[index] = pInput->getDataType();
        else
            m_originalInputTypes[index] = IODataType::NONE;
    }

    if(pInput == nullptr && m_inputs[index] != nullptr)
        m_inputs[index]->clearData();

    if(Utils::Workflow::isConvertibleIO(m_inputs[index], pInput))
    {
        //Just share pointer
        CPyEnsureGIL gil;
        m_inputs[index] = pInput;
    }
    else
    {
        //In case of impossible implicit conversion between input/ouput types
        //we should use this copy mecanism to give the possibility of derived
        //CWorkflowTaskIO classes to define the right behavior to convert input
        //and output (ex: CGraphicsInput and CGraphicsOutput)
        m_inputs[index]->copy(pInput);
    }
    updateStaticOutputs();
}

void CWorkflowTask::setInputs(const InputOutputVect &inputs, bool bNewSequence)
{
    Q_UNUSED(bNewSequence);
    m_inputs = inputs;
    m_inputViewProps.resize(m_inputs.size());
    updateStaticOutputs();
}

void CWorkflowTask::setOutputDataType(const IODataType &dataType, size_t index)
{
    if(index < m_outputs.size())
        m_outputs[index]->setDataType(dataType);
    else
    {
        m_outputs.resize(index + 1);
        m_outputs[index]->setDataType(dataType);
    }
}

void CWorkflowTask::setOutput(const WorkflowTaskIOPtr &pOutput, size_t index)
{
    if(index < m_outputs.size())
        m_outputs[index] = pOutput;
    else
    {
        m_outputs.resize(index + 1);
        m_outputs[index] = pOutput;
        m_outputViewProps.emplace_back(CViewPropertyIO());
    }
}

void CWorkflowTask::setOutputs(const InputOutputVect &outputs)
{
    m_outputs = outputs;
    m_outputViewProps.resize(m_outputs.size());
}

void CWorkflowTask::setParam(const WorkflowTaskParamPtr &pParam)
{
    m_pParam = pParam;
}

void CWorkflowTask::setActive(bool bActive)
{
    m_bActive = bActive;
}

void CWorkflowTask::setParamMap(const UMapString &paramMap)
{
    if(m_pParam)
        m_pParam->setParamMap(paramMap);
}

void CWorkflowTask::setName(const std::string &name)
{
    m_name = name;
}

void CWorkflowTask::setOutputFolder(const std::string &folder)
{
    m_outputFolder = folder;
}

void CWorkflowTask::setActionFlag(CWorkflowTask::ActionFlag flag, bool bEnable)
{
    auto it = m_actionFlags.find(flag);
    if(it != m_actionFlags.end())
        it->second = bEnable;
    else
        m_actionFlags.insert(std::make_pair(flag, bEnable));
}

void CWorkflowTask::setGraphicsContext(const GraphicsContextPtr &contextPtr)
{
    m_graphicsContextPtr = contextPtr;
}

void CWorkflowTask::setBatchInput(bool bBatch)
{
    if(bBatch)
        setActionFlag(ActionFlag::OUTPUT_AUTO_EXPORT, isAutoSave());
    else
        removeActionFlag(ActionFlag::OUTPUT_AUTO_EXPORT);
}

void CWorkflowTask::setAutoSave(bool bEnable)
{
    for(size_t i=0; i<m_outputs.size(); ++i)
        m_outputs[i]->setAutoSave(bEnable);
}

CWorkflowTask::Type CWorkflowTask::getType() const
{
    return m_type;
}

std::string CWorkflowTask::getName() const
{
    return m_name;
}

size_t CWorkflowTask::getInputCount() const
{
    return m_inputs.size();
}

size_t CWorkflowTask::getValidInputCount() const
{
    size_t nb = 0;
    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        if(m_inputs[i] != nullptr)
            nb++;
    }
    return nb;
}

InputOutputVect CWorkflowTask::getInputs() const
{
    return m_inputs;
}

InputOutputVect CWorkflowTask::getInputs(const std::set<IODataType> &types) const
{
    InputOutputVect inputs;

    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        auto it = types.find(m_inputs[i]->getDataType());
        if(it != types.end())
            inputs.push_back(m_inputs[i]);
    }
    return inputs;
}

WorkflowTaskIOPtr CWorkflowTask::getInput(size_t index) const
{
    if(index < m_inputs.size())
        return m_inputs[index];
    else
        return nullptr;
}

IODataType CWorkflowTask::getInputDataType(size_t index) const
{
    if(index < m_inputs.size() && m_inputs[index] != nullptr)
        return m_inputs[index]->getDataType();
    else
        return IODataType::NONE;
}

IODataType CWorkflowTask::getOriginalInputDataType(size_t index) const
{
    if(index < m_originalInputTypes.size())
        return m_originalInputTypes[index];
    else
        return IODataType::NONE;
}

size_t CWorkflowTask::getOutputCount() const
{
    return m_outputs.size();
}

InputOutputVect CWorkflowTask::getOutputs() const
{
    return m_outputs;
}

InputOutputVect CWorkflowTask::getOutputs(const std::set<IODataType> &dataTypes) const
{
    InputOutputVect outputs;
    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        auto it = dataTypes.find(m_outputs[i]->getDataType());
        if(it != dataTypes.end())
            outputs.push_back(m_outputs[i]);
    }
    return outputs;
}

WorkflowTaskIOPtr CWorkflowTask::getOutput(size_t index) const
{
    if(index < m_outputs.size())
        return m_outputs[index];
    else
        return nullptr;
}

IODataType CWorkflowTask::getOutputDataType(size_t index) const
{
    if(index < m_outputs.size())
        return m_outputs[index]->getDataType();
    else
        return IODataType::NONE;
}

std::string CWorkflowTask::getOutputFolder() const
{
    return m_outputFolder;
}

WorkflowTaskParamPtr CWorkflowTask::getParam() const
{
    return m_pParam;
}

uint CWorkflowTask::getHashValue() const
{
    if(m_pParam)
        return m_pParam->getHashValue();
    else
        return 0;
}

size_t CWorkflowTask::getProgressSteps()
{
    return 0;
}

size_t CWorkflowTask::getProgressSteps(size_t unitEltCount)
{
    if(isActionFlagEnable(ActionFlag::APPLY_VOLUME) ||
            hasInput(IODataType::VIDEO) ||
            hasInput(IODataType::VIDEO_BINARY) ||
            hasInput(IODataType::VIDEO_LABEL))
        return unitEltCount * getProgressSteps();
    else
        return getProgressSteps();
}

double CWorkflowTask::getElapsedTime() const
{
    return m_elapsedTime;
}

VectorPairString CWorkflowTask::getCustomInfo() const
{
    return m_customInfo;
}

WorkflowTaskIOPtr CWorkflowTask::getOutputFromType(const IODataType &type) const
{
    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        if(m_outputs[i]->getDataType() == type)
            return m_outputs[i];
    }
    return nullptr;
}

std::map<CWorkflowTask::ActionFlag,bool> CWorkflowTask::getActionFlags() const
{
    return m_actionFlags;
}

CViewPropertyIO *CWorkflowTask::getInputViewProperty(size_t index)
{
    if(index < m_inputViewProps.size())
        return &m_inputViewProps[index];
    else
        return nullptr;
}

CViewPropertyIO *CWorkflowTask::getOutputViewProperty(size_t index)
{
    if(index < m_outputViewProps.size())
        return &m_outputViewProps[index];
    else
        return nullptr;
}

CSignalHandler *CWorkflowTask::getSignalRawPtr()
{
    return m_signalHandler.get();
}

bool CWorkflowTask::isActionFlagEnable(CWorkflowTask::ActionFlag flag) const
{
    auto it = m_actionFlags.find(flag);
    if(it == m_actionFlags.end())
        return false;
    else
        return it->second;
}

bool CWorkflowTask::isRunning() const
{
    return m_bRunning;
}

bool CWorkflowTask::isGraphicsChangedListening() const
{
    return false;
}

bool CWorkflowTask::isSelfInput() const
{
    if(m_inputs.size() == 0)
        return true;
    else
    {
        for(size_t i=0; i<m_inputs.size(); ++i)
        {
            if(m_inputs[i] == nullptr)
                continue;

            if(!m_inputs[i]->isAutoInput())
                return false;
        }
        return true;
    }
}

bool CWorkflowTask::isAutoSave() const
{
    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        if(m_outputs[i]->isAutoSave() == true)
            return true;
    }
    return false;
}

bool CWorkflowTask::hasOutput(const IODataType &type) const
{
    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        if(m_outputs[i]->getDataType() == type)
            return true;
    }
    return false;
}

bool CWorkflowTask::hasOutput(const std::set<IODataType> &types) const
{
    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        auto it = types.find(m_outputs[i]->getDataType());
        if(it != types.end())
            return true;
    }
    return false;
}

bool CWorkflowTask::hasOutputData() const
{
    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        if(m_outputs[i]->isDataAvailable() == true)
            return true;
    }
    return false;
}

bool CWorkflowTask::hasInput(const IODataType& type) const
{
    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        if(m_inputs[i] != nullptr && m_inputs[i]->getDataType() == type)
            return true;
    }
    return false;
}

bool CWorkflowTask::hasInput(const std::set<IODataType> &types) const
{
    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        if(m_inputs[i] == nullptr)
            continue;

        auto it = types.find(m_inputs[i]->getDataType());
        if(it != types.end())
            return true;
    }
    return false;
}

void CWorkflowTask::addInput(const WorkflowTaskIOPtr &pInput)
{
    m_inputs.emplace_back(pInput);
    m_inputViewProps.emplace_back(CViewPropertyIO());

    if(pInput)
        m_originalInputTypes.push_back(pInput->getDataType());
    else
        m_originalInputTypes.push_back(IODataType::NONE);
}

void CWorkflowTask::addInput(const WorkflowTaskIOPtr&& pInput)
{
    m_inputs.emplace_back(pInput);
    m_inputViewProps.emplace_back(CViewPropertyIO());

    if(pInput)
        m_originalInputTypes.push_back(pInput->getDataType());
    else
        m_originalInputTypes.push_back(IODataType::NONE);
}

void CWorkflowTask::addOutput(const WorkflowTaskIOPtr &pOutput)
{
    m_outputs.emplace_back(pOutput);
    m_outputViewProps.emplace_back(CViewPropertyIO());
}

void CWorkflowTask::addOutput(const WorkflowTaskIOPtr&& pOutput)
{
    m_outputs.emplace_back(pOutput);
    m_outputViewProps.emplace_back(CViewPropertyIO());
}

void CWorkflowTask::addInputs(const InputOutputVect &inputs)
{
    m_inputs.insert(m_inputs.end(), inputs.begin(), inputs.end());
    m_inputViewProps.resize(m_inputs.size());

    for(size_t i=0; i<inputs.size(); ++i)
    {
        if(inputs[i] != nullptr)
            m_originalInputTypes.push_back(inputs[i]->getDataType());
        else
            m_originalInputTypes.push_back(IODataType::NONE);
    }
}

void CWorkflowTask::addOutputs(const InputOutputVect &outputs)
{
    m_outputs.insert(m_outputs.end(), outputs.begin(), outputs.end());
    m_outputViewProps.resize(m_inputs.size());
}

void CWorkflowTask::insertInput(const WorkflowTaskIOPtr &pInput, size_t index)
{
    if(index >= m_inputs.size())
    {
        m_inputs.push_back(pInput);
        m_inputViewProps.emplace_back(CViewPropertyIO());

        if(pInput)
            m_originalInputTypes.push_back(pInput->getDataType());
        else
            m_originalInputTypes.push_back(IODataType::NONE);
    }
    else
    {
        m_inputs.insert(m_inputs.begin() + index, pInput);
        m_inputViewProps.insert(m_inputViewProps.begin() + index, CViewPropertyIO());

        if(pInput)
            m_originalInputTypes.insert(m_originalInputTypes.begin() + index, pInput->getDataType());
        else
            m_originalInputTypes.insert(m_originalInputTypes.begin() + index, IODataType::NONE);
    }
}

void CWorkflowTask::insertOutput(const WorkflowTaskIOPtr &pOutput, size_t index)
{
    if(index >= m_outputs.size())
    {
        m_outputs.push_back(pOutput);
        m_outputViewProps.emplace_back(CViewPropertyIO());
    }
    else
    {
        m_outputs.insert(m_outputs.begin() + index, pOutput);
        m_outputViewProps.insert(m_outputViewProps.begin() + index, CViewPropertyIO());
    }
}

void CWorkflowTask::clearInputs()
{
    m_inputs.clear();
    m_originalInputTypes.clear();
    m_inputViewProps.clear();
}

void CWorkflowTask::clearOutputs()
{
    m_outputs.clear();
    m_outputViewProps.clear();
}

void CWorkflowTask::clearInputData(size_t index)
{
    if(index < m_inputs.size() && m_inputs[index] != nullptr)
        m_inputs[index]->clearData();
}

void CWorkflowTask::clearOutputData()
{
    for(size_t i=0; i<m_outputs.size(); ++i)
        if(m_outputs[i] != nullptr)
            m_outputs[i]->clearData();
}

void CWorkflowTask::resetInput(size_t index, WorkflowTaskIOPtr& io)
{
    if(index < m_inputs.size())
        m_inputs[index] = io;
}

void CWorkflowTask::removeInput(size_t index)
{
    if(index < m_inputs.size())
    {
        m_inputs.erase(m_inputs.begin() + index);
        m_originalInputTypes.erase(m_originalInputTypes.begin() + index);
        m_inputViewProps.erase(m_inputViewProps.begin() + index);
    }
}

void CWorkflowTask::removeOutput(size_t index)
{
    if(index < m_outputs.size())
    {
        m_outputs.erase(m_outputs.begin() + index);
        m_outputViewProps.erase(m_outputViewProps.begin() + index);
    }
}

void CWorkflowTask::removeActionFlag(CWorkflowTask::ActionFlag flag)
{
    auto it = m_actionFlags.find(flag);
    if(it != m_actionFlags.end())
        m_actionFlags.erase(it);
}

std::vector<TaskIOLockerUPtr> CWorkflowTask::createInputScopedLocks() const
{
    std::vector<TaskIOLockerUPtr> locks;
    InputOutputSet uniqueInputs(m_inputs.begin(), m_inputs.end());

    //Use of unique pointer to ensure the lock is get once
    for(auto it=uniqueInputs.begin(); it!=uniqueInputs.end(); ++it)
    {
        if(*it != nullptr)
            locks.push_back(std::make_unique<CObjectLocker<CWorkflowTaskIO>>(**it));
    }
    return locks;
}

std::vector<TaskIOLockerUPtr> CWorkflowTask::createOutputScopedLocks() const
{
    std::vector<TaskIOLockerUPtr> locks;
    InputOutputSet uniqueOutputs(m_outputs.begin(), m_outputs.end());

    //Use of unique pointer to ensure the lock is get once
    for(auto it=uniqueOutputs.begin(); it!=uniqueOutputs.end(); ++it)
        locks.push_back(std::make_unique<CObjectLocker<CWorkflowTaskIO>>(**it));

    return locks;
}

void CWorkflowTask::run()
{
    m_outputs = m_inputs;
}

void CWorkflowTask::executeActions(int flags)
{
    Q_UNUSED(flags);
}

void CWorkflowTask::updateStaticOutputs()
{
    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        if(i < m_outputs.size() && m_outputs[i] != nullptr)
            m_outputs[i]->copyStaticData(m_inputs[i]);
    }
}

void CWorkflowTask::beginTaskRun()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_bRunning = true;
    m_bStop = false;

    //Check inputs validity
    for(size_t i=0; i<m_inputs.size(); ++i)
        if(m_inputs[i] == nullptr)
            throw CException(CoreExCode::INVALID_PARAMETER, "Invalid input for task " + m_name + ". Please check connections and data type.", __func__, __FILE__, __LINE__);

    if(!isSelfInput() && m_inputs.size() == 0)
        throw CException(CoreExCode::INVALID_PARAMETER, "No available input for task " + m_name + ".", __func__, __FILE__, __LINE__);

    //Notify current task name
    emit m_signalHandler->doSetMessage(QString::fromStdString(m_name));

    //Start timing
    m_timer.start();
}

void CWorkflowTask::endTaskRun()
{
    //Compute and emit task running time
    m_elapsedTime = m_timer.get_total_elapsed_ms();
    emit m_signalHandler->doSetElapsedTime(m_elapsedTime);
    m_bRunning = false;
}

void CWorkflowTask::parametersModified()
{
}

void CWorkflowTask::graphicsChanged()
{
}

void CWorkflowTask::globalInputChanged(bool bNewSequence)
{
    Q_UNUSED(bNewSequence);
}

void CWorkflowTask::notifyVideoStart(int)
{
}

void CWorkflowTask::notifyVideoEnd()
{
}

void CWorkflowTask::stop()
{
    m_bStop = true;
}

void CWorkflowTask::saveOutputs(const std::string& baseName) const
{
    bool bFirst = true;
    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        if(m_outputs[i]->isAutoSave())
        {
            if(bFirst)
            {
                Utils::File::createDirectory(m_outputFolder);
                bFirst = false;
            }
            m_outputs[i]->setSaveInfo(m_outputFolder, baseName);
            m_outputs[i]->save();
        }
    }
}

QJsonObject CWorkflowTask::toJson() const
{
    QJsonObject obj;
    obj["name"] = QString::fromStdString(m_name);    

    // Associated parameters
    QJsonArray jsonParams;
    auto paramMap = m_pParam->getParamMap();

    for(auto it=paramMap.begin(); it!=paramMap.end(); ++it)
    {
        QJsonObject jsonParam;
        jsonParam["name"] = QString::fromStdString(it->first);
        jsonParam["value"] = QString::fromStdString(it->second);
        jsonParams.append(jsonParam);
    }
    obj["parameters"] = jsonParams;
    return obj;
}

#include "moc_CWorkflowTask.cpp"
