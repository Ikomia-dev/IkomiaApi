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

#include <QObject>
#include "CWorkflow.h"
#include "CException.h"
#include <QDebug>
#include <QFile>
#include <unordered_map>
#include <boost/graph/graphviz.hpp>
#include "Main/CoreTools.hpp"
#include "Graphics/CGraphicsLayer.h"
#include "IO/CGraphicsInput.h"

//-------------------//
//- Class CWorkflow -//
//-------------------//
CWorkflow::CWorkflow() : CWorkflowTask()
{
    createRoot();
    m_lastTaskAdded = m_root;
    m_activeTask = m_root;
    m_runningTask = boost::graph_traits<WorkflowGraph>::null_vertex();
    m_signalHandler = std::make_unique<CWorkflowSignalHandler>();
    m_runMgr.setCfg(&m_cfg);
}

CWorkflow::CWorkflow(const std::string &name) : CWorkflowTask(name)
{
    createRoot();
    m_lastTaskAdded = m_root;
    m_activeTask = m_root;
    m_runningTask = boost::graph_traits<WorkflowGraph>::null_vertex();
    m_signalHandler = std::make_unique<CWorkflowSignalHandler>();
    m_runMgr.setCfg(&m_cfg);
}

CWorkflow::CWorkflow(const std::string &name, CProcessRegistration *pTaskRegistration, CTaskIORegistration *pIORegistration, const GraphicsContextPtr &contextPtr)
    : CWorkflowTask(name)
{
    createRoot();
    m_lastTaskAdded = m_root;
    m_activeTask = m_root;
    m_runningTask = boost::graph_traits<WorkflowGraph>::null_vertex();
    m_signalHandler = std::make_unique<CWorkflowSignalHandler>();
    m_runMgr.setCfg(&m_cfg);
    m_pTaskRegistration = pTaskRegistration;
    m_pTaskIORegistration = pIORegistration;
    m_graphicsContextPtr = contextPtr;
}

CWorkflow::CWorkflow(const CWorkflow &workflow) : CWorkflowTask(workflow)
{
    m_graph = workflow.m_graph;
    m_root = workflow.m_root;
    m_hashValue = workflow.m_hashValue;
    m_description = workflow.m_description;
    m_keywords = workflow.m_keywords;
    m_lastTaskAdded = workflow.m_lastTaskAdded;
    m_activeTask = workflow.m_activeTask;
    m_runningTask = workflow.m_runningTask;
    m_bAutoSave = workflow.m_bAutoSave;
    m_signalHandler = std::make_unique<CWorkflowSignalHandler>();
    m_runMgr.setCfg(&m_cfg);
}

CWorkflow::CWorkflow(const CWorkflow&& workflow) : CWorkflowTask(workflow)
{
    m_graph = std::move(workflow.m_graph);
    m_root = std::move(workflow.m_root);
    m_hashValue = std::move(workflow.m_hashValue);
    m_description = std::move(workflow.m_description);
    m_keywords = std::move(workflow.m_keywords);
    m_lastTaskAdded = std::move(workflow.m_lastTaskAdded);
    m_activeTask = std::move(workflow.m_activeTask);
    m_runningTask = std::move(workflow.m_runningTask);
    m_bAutoSave = std::move(workflow.m_bAutoSave);
    m_signalHandler = std::make_unique<CWorkflowSignalHandler>();
    m_runMgr.setCfg(&m_cfg);
}

CWorkflow::~CWorkflow()
{
    CPyEnsureGIL gil;
    m_graph.clear();
}

CWorkflow &CWorkflow::operator=(const CWorkflow &workflow)
{
    CWorkflowTask::operator=(workflow);
    m_graph = workflow.m_graph;
    m_root = workflow.m_root;
    m_hashValue = workflow.m_hashValue;
    m_description = workflow.m_description;
    m_keywords = workflow.m_keywords;
    m_lastTaskAdded = workflow.m_lastTaskAdded;
    m_activeTask = workflow.m_activeTask;
    m_runningTask = workflow.m_runningTask;
    m_bAutoSave = workflow.m_bAutoSave;
    return *this;
}

CWorkflow &CWorkflow::operator=(const CWorkflow&& workflow)
{
    CWorkflowTask::operator=(workflow);
    m_graph = std::move(workflow.m_graph);
    m_root = std::move(workflow.m_root);
    m_hashValue = std::move(workflow.m_hashValue);
    m_description = std::move(workflow.m_description);
    m_keywords = std::move(workflow.m_keywords);
    m_lastTaskAdded = std::move(workflow.m_lastTaskAdded);
    m_activeTask = std::move(workflow.m_activeTask);
    m_runningTask = std::move(workflow.m_runningTask);
    m_bAutoSave = std::move(workflow.m_bAutoSave);
    return *this;
}

WorkflowTaskPtr CWorkflow::operator[](WorkflowVertex v)
{
    return m_graph[v];
}

WorkflowEdgePtr CWorkflow::operator[](WorkflowEdge e)
{
    return m_graph[e];
}

/***********/
/* SETTERS */
/***********/

void CWorkflow::setDescription(const std::string &description)
{
    m_description = description;
}

void CWorkflow::setKeywords(const std::string &keywords)
{
    m_keywords = keywords;
}

void CWorkflow::setInput(const WorkflowTaskIOPtr &pInput, size_t index, bool bNewSequence)
{
    CWorkflowTask::setInput(pInput, index, bNewSequence);

    auto pRootTask = m_graph[m_root];
    pRootTask->setInput(pInput, index);
    pRootTask->setOutput(pInput, index);

    auto pActiveTask = m_graph[m_activeTask];
    if(pActiveTask)
        pActiveTask->globalInputChanged(bNewSequence);

    if(bNewSequence && !m_bForceBatchMode)
        checkBatchModeState();

    startIOAnalysis(m_root);
}

void CWorkflow::setInputs(const InputOutputVect &inputs, bool bNewSequence)
{
    CWorkflowTask::setInputs(inputs, bNewSequence);

    auto pRootTask = m_graph[m_root];
    pRootTask->setInputs(inputs, bNewSequence);
    pRootTask->setOutputs(inputs);

    auto pActiveTask = m_graph[m_activeTask];
    if(pActiveTask)
        pActiveTask->globalInputChanged(bNewSequence);

    if(bNewSequence)
        checkBatchModeState();

    startIOAnalysis(m_root);
}

void CWorkflow::setOutputFolder(const std::string &folder)
{
    m_outputFolder = folder;
    auto pRootTask = m_graph[m_root];
    pRootTask->setOutputFolder(folder);
}

void CWorkflow::setActiveTask(const WorkflowVertex &id)
{
    //Desactivate previous task
    auto taskPtr = getTask(m_activeTask);
    if(taskPtr)
        taskPtr->setActive(false);

    if(id == boost::graph_traits<WorkflowGraph>::null_vertex())
        m_activeTask = m_root;
    else
        m_activeTask = id;

    //Activate new one
    taskPtr = getTask(m_activeTask);
    if(taskPtr)
        taskPtr->setActive(true);
}

void CWorkflow::setRunningTask(const WorkflowVertex &id)
{
    m_runningTask = id;
}

void CWorkflow::setTaskActionFlag(const WorkflowVertex &id, CWorkflowTask::ActionFlag action, bool bEnable)
{
    auto taskPtr = m_graph[id];
    if(taskPtr != nullptr)
    {
        taskPtr->setActionFlag(action, bEnable);
        if(action == ActionFlag::OUTPUT_AUTO_EXPORT)
            taskPtr->setAutoSave(bEnable);

        startIOAnalysis(id);
    }
}

void CWorkflow::setInputBatchState(size_t index, bool bBatch)
{
    if(index >= m_inputBatchState.size())
        m_inputBatchState.resize(index+1);

    m_inputBatchState[index] = bBatch;
}

void CWorkflow::setCfgEntry(const std::string &key, const std::string &value)
{
    m_cfg[key] = value;
}

void CWorkflow::setAutoSave(bool bEnable)
{
    m_bAutoSave = bEnable;
    auto vertexRangeIt = boost::vertices(m_graph);

    for(auto it=vertexRangeIt.first; it!=vertexRangeIt.second; ++it)
    {
        WorkflowTaskPtr taskPtr = m_graph[*it];
        if(*it != m_root && taskPtr)
            taskPtr->setAutoSave(m_bAutoSave);
    }
}

/***********/
/* GETTERS */
/***********/

std::string CWorkflow::getDescription() const
{
    return m_description;
}

std::string CWorkflow::getKeywords() const
{
    return m_keywords;
}

WorkflowVertex CWorkflow::getRootId() const
{
    return m_root;
}

size_t CWorkflow::getTaskCount() const
{
    return boost::num_vertices(m_graph);
}

size_t CWorkflow::getChildCount(const WorkflowVertex &parent) const
{
     if(parent == boost::graph_traits<WorkflowGraph>::null_vertex())
         return boost::out_degree(m_root, m_graph);
     else
         return boost::out_degree(parent, m_graph);
}

size_t CWorkflow::getChildIndex(const WorkflowVertex &parent, const WorkflowVertex &child) const
{
    int index = 0;
    auto edgeRangeIt = boost::out_edges(parent, m_graph);

    for(auto it=edgeRangeIt.first; it!=edgeRangeIt.second; ++it)
    {
        auto childIdTmp = boost::target(*it, m_graph);
        if(childIdTmp == child)
            return index;

        index++;
    }
    return index;
}

WorkflowVertex CWorkflow::getLastTaskId() const
{
    return m_lastTaskAdded;
}

WorkflowVertex CWorkflow::getActiveTaskId() const
{
    return m_activeTask;
}

WorkflowVertex CWorkflow::getRunningTaskId() const
{
    return m_runningTask;
}

WorkflowVertex CWorkflow::getTaskId(const std::string &name) const
{
    auto rangeIt = boost::vertices(m_graph);
    for(auto it = rangeIt.first; it != rangeIt.second; ++it)
    {
        auto taskPtr = m_graph[*it];
        if(taskPtr != nullptr && taskPtr->getName() == name)
            return *it;
    }
    return boost::graph_traits<WorkflowGraph>::null_vertex();
}

WorkflowTaskPtr CWorkflow::getTask(const WorkflowVertex &id)
{
    if(id == boost::graph_traits<WorkflowGraph>::null_vertex())
        return m_graph[m_root];
    else
        return m_graph[id];
}

std::vector<WorkflowVertex> CWorkflow::getParents(const WorkflowVertex &id) const
{
    std::vector<WorkflowVertex> parents;
    auto edgeIt = boost::in_edges(id, m_graph);

    for(auto it=edgeIt.first; it!=edgeIt.second; ++it)
    {
        auto v = boost::source(*it, m_graph);
        if(std::find(parents.begin(), parents.end(), v) == parents.end())
            parents.push_back(v);
    }

    return parents;
}

void CWorkflow::getAllParents(const WorkflowVertex &id, std::vector<WorkflowVertex>& parents) const
{
    if(id == m_root)
        return;

    auto directParents = getParents(id);
    for(auto& it : directParents)
    {
        parents.push_back(it);
        getAllParents(it, parents);
    }
}

std::vector<WorkflowVertex> CWorkflow::getChilds(const WorkflowVertex &parent) const
{
    std::vector<WorkflowVertex> childs;
    std::pair<boost::graph_traits<WorkflowGraph>::out_edge_iterator, boost::graph_traits<WorkflowGraph>::out_edge_iterator> edges;

    if(parent == boost::graph_traits<WorkflowGraph>::null_vertex())
        edges = boost::out_edges(m_root, m_graph);
    else
        edges = boost::out_edges(parent, m_graph);

    for(auto edgeIt=edges.first; edgeIt!=edges.second; ++edgeIt)
    {
        WorkflowVertex child = boost::target(*edgeIt, m_graph);
        auto it = std::find(childs.begin(), childs.end(), child);

        if(it == childs.end())
            childs.push_back(child);
    }
    return childs;
}

std::vector<WorkflowVertex> CWorkflow::getAllChilds(const WorkflowVertex& parent) const
{
    VertexIndexMap mapIndex;
    auto propMapIndex = createBfsPropertyMap(mapIndex);
    auto pChilds = std::make_shared<std::vector<WorkflowVertex>>();
    CFindChildVisitor visitor(pChilds);

    if(parent == boost::graph_traits<WorkflowGraph>::null_vertex())
        boost::breadth_first_search(m_graph, m_root, boost::visitor(visitor).vertex_index_map(propMapIndex));
    else
        boost::breadth_first_search(m_graph, parent, boost::visitor(visitor).vertex_index_map(propMapIndex));

    return *pChilds;
}

std::vector<WorkflowTaskPtr> CWorkflow::getFinalTasks()
{
    std::vector<WorkflowTaskPtr> finalTasks;

    auto pairIt = boost::vertices(m_graph);
    for(auto it = pairIt.first; it!=pairIt.second; ++it)
    {
        if(boost::out_degree(*it, m_graph) == 0)
            finalTasks.push_back(m_graph[*it]);
    }
    return finalTasks;
}

WorkflowVertexRangeIt CWorkflow::getVertices()
{
    return boost::vertices(m_graph);
}

WorkflowEdgeRangeIt CWorkflow::getEdges()
{
    return boost::edges(m_graph);
}

WorkflowInEdgeRangeIt CWorkflow::getInEdges(const WorkflowVertex &id)
{
    return boost::in_edges(id, m_graph);
}

size_t CWorkflow::getInEdgesCount(const WorkflowVertex &id) const
{
    size_t count = 0;
    auto rangeIt = boost::in_edges(id, m_graph);

    for(auto it=rangeIt.first; it!=rangeIt.second; ++it)
        count++;

    return count;
}

WorkflowOutEdgeRangeIt CWorkflow::getOutEdges(const WorkflowVertex &id)
{
    return boost::out_edges(id, m_graph);
}

size_t CWorkflow::getOutEdgeCount(const WorkflowVertex &id) const
{
    size_t count = 0;
    auto rangeIt = boost::out_edges(id, m_graph);

    for(auto it=rangeIt.first; it!=rangeIt.second; ++it)
        count++;

    return count;
}

WorkflowEdgePtr CWorkflow::getEdge(const WorkflowEdge &id) const
{
    return m_graph[id];
}

WorkflowVertex CWorkflow::getEdgeSource(const WorkflowEdge &id)
{
    return boost::source(id, m_graph);
}

WorkflowVertex CWorkflow::getEdgeTarget(const WorkflowEdge &id)
{
    return boost::target(id, m_graph);
}

size_t CWorkflow::getProgressSteps()
{
    return getProgressStepsFrom(m_root);
}

size_t CWorkflow::getProgressStepsFrom(const WorkflowVertex &idFrom) const
{
    if(getTaskCount() == 0)
        return 0;

    size_t steps = 0;
    size_t unitEltNb = 1;

    for(size_t i=0; i<getInputCount(); ++i)
        unitEltNb = std::max(unitEltNb, getInput(i)->getUnitElementCount());

    VertexIndexMap mapIndex;
    auto propMapIndex = createBfsPropertyMap(mapIndex);
    CProgressStepVisitor visitor(unitEltNb, steps);
    boost::breadth_first_search(m_graph, idFrom, boost::visitor(visitor).vertex_index_map(propMapIndex));

    //Manage self input tasks => orphans not accessible by bfs algorithm
    auto selfInputTasks = getSelfInputTasks();
    for(size_t i=0; i<selfInputTasks.size(); ++i)
    {
        auto childs = getAllChilds(selfInputTasks[i]);
        for(size_t j=0; j<childs.size(); ++j)
        {
            auto taskPtr = m_graph[childs[j]];
            steps += taskPtr->getProgressSteps(unitEltNb);
        }
    }
    return steps;
}

size_t CWorkflow::getProgressStepsTo(const WorkflowVertex &idTo) const
{
    // Search for and run all task not already executed before id
    std::vector<WorkflowVertex> taskToExecute;
    taskToExecute.push_back(idTo);
    findTaskToExecute(taskToExecute, idTo);
    return getProgressSteps(taskToExecute);
}

size_t CWorkflow::getProgressSteps(const std::vector<WorkflowVertex> &tasks) const
{
    size_t steps = 0;
    size_t unitEltNb = 1;

    for(size_t i=0; i<getInputCount(); ++i)
        unitEltNb = std::max(unitEltNb, getInput(i)->getUnitElementCount());

    for(size_t i=0; i<tasks.size(); ++i)
    {
        auto pTask = m_graph[tasks[i]];
        steps += pTask->getProgressSteps(unitEltNb);
    }
    return steps;
}

GraphicsInputPtr CWorkflow::getGraphicsInput(const WorkflowTaskPtr &pTask)
{
    for(size_t i=0; i<pTask->getInputCount(); ++i)
    {
        auto pInput = pTask->getInput(i);
        if(pInput && pInput->getDataType() == IODataType::INPUT_GRAPHICS)
            return std::static_pointer_cast<CGraphicsInput>(pInput);
    }
    return nullptr;
}

double CWorkflow::getElapsedTimeTo(const WorkflowVertex& id)
{
    if(id == boost::graph_traits<WorkflowGraph>::null_vertex())
        return 0.0;

    // Search for all task before id
    std::vector<WorkflowVertex> taskToExecute;
    taskToExecute.push_back(id);
    findPreviousTasks(taskToExecute, id);

    double elapsedTime = 0.0;
    for(auto vertexIt=taskToExecute.begin(); vertexIt!=taskToExecute.end(); ++vertexIt)
    {
        auto pTask = m_graph[*vertexIt];
        if(pTask == nullptr)
            throw CException(CoreExCode::INVALID_PARAMETER, "Null pointer exception", __func__, __FILE__, __LINE__);
        else
        {
            elapsedTime += pTask->getElapsedTime();
        }
    }

    return elapsedTime;
}

uint CWorkflow::getHashValue() const
{
    QSet<uint> hashValues;
    hashValues.reserve(static_cast<int>(boost::num_vertices(m_graph) + boost::num_edges(m_graph)));

    auto vertexRangeIt = boost::vertices(m_graph);
    for(auto it=vertexRangeIt.first; it!=vertexRangeIt.second; ++it)
        hashValues.insert(qHash(qMakePair(boost::hash_value(*it), m_graph[*it]->getHashValue())));

    auto edgeRangeIt = boost::edges(m_graph);
    for(auto it=edgeRangeIt.first; it!=edgeRangeIt.second; ++it)
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, boost::source(*it, m_graph));
        boost::hash_combine(seed, boost::target(*it, m_graph));
        hashValues.insert(qHash(qMakePair(seed, m_graph[*it]->getHashValue())));
    }
    return qHash(hashValues);
}

std::vector<WorkflowVertex> CWorkflow::getForwardPassTasks(const WorkflowVertex &startVertex)
{
    std::vector<WorkflowVertex> tasks;
    std::list<WorkflowVertex> candidates;

    tasks.push_back(startVertex);
    auto childs = getChilds(startVertex);

    //Add 'self input' task with no input connection
    if(startVertex == m_root)
    {
        auto selfInputTasks = getSelfInputTasks();
        candidates.insert(candidates.end(), selfInputTasks.begin(), selfInputTasks.end());
    }

    for(size_t i=0; i<childs.size(); ++i)
        candidates.push_back(childs[i]);

    while(candidates.size() > 0)
    {
        auto candidate = candidates.front();
        candidates.pop_front();
        WorkflowTaskPtr taskPtr = m_graph[candidate];
        bool bValidTask = (!taskPtr->isSelfInput() || (taskPtr->isSelfInput() && !taskPtr->hasOutputData()));
        auto parents = getParents(candidate);
        bool bValidParents = true;

        //Check if all parents are already added
        for(size_t i=0; i<parents.size() && bValidParents == true; ++i)
        {
            auto it = std::find(tasks.begin(), tasks.end(), parents[i]);
            if(it == tasks.end())
                bValidParents = false;
        }

        if(!bValidParents)
        {
            std::vector<WorkflowVertex> neededParents;
            findTaskToExecute(neededParents, candidate);

            // Add needed parents if not already in
            for(size_t i=0; i<neededParents.size(); ++i)
            {
                auto it = std::find(tasks.begin(), tasks.end(), neededParents[i]);
                if(it == tasks.end())
                    tasks.push_back(neededParents[i]);
            }

            //Add candidate task
            if(bValidTask)
                tasks.push_back(candidate);
        }
        else
        {
            //Add candidate task
            if(bValidTask)
            {
                auto it = std::find(tasks.begin(), tasks.end(), candidate);
                if(it == tasks.end())
                    tasks.push_back(candidate);
            }

            //Insert each child to the FIFO list if not already in
            childs = getChilds(candidate);
            for(size_t i=0; i<childs.size(); ++i)
            {
                auto it = std::find(candidates.begin(), candidates.end(), childs[i]);
                if(it == candidates.end())
                    candidates.push_back(childs[i]);
            }
        }
    }
    return tasks;
}

std::vector<WorkflowVertex> CWorkflow::getSelfInputTasks() const
{
    std::vector<WorkflowVertex> tasks;
    auto vertices = boost::vertices(m_graph);

    for(auto it=vertices.first; it!=vertices.second; ++it)
    {
        if(*it != m_root && getInEdgesCount(*it) == 0)
        {
            WorkflowTaskPtr taskPtr = m_graph[*it];
            if(taskPtr && taskPtr->isSelfInput())
                tasks.push_back(*it);
        }
    }
    return tasks;
}

double CWorkflow::getTotalElapsedTime() const
{
    VertexIndexMap mapIndex;
    auto propMapIndex = createBfsPropertyMap(mapIndex);
    double elapsedTime = 0.0;
    CElapsedTimeVisitor visitor(elapsedTime);
    boost::breadth_first_search(m_graph, m_root, boost::visitor(visitor).vertex_index_map(propMapIndex));

    return elapsedTime;
}

CDataInfoPtr CWorkflow::getIOInfo(const WorkflowVertex &id, size_t index, bool bInput)
{
    auto taskPtr = getTask(id);
    if(!taskPtr)
        return nullptr;

    WorkflowTaskIOPtr ioPtr;
    if(bInput)
        ioPtr = taskPtr->getInput(index);
    else
        ioPtr = taskPtr->getOutput(index);

    if(ioPtr)
        return ioPtr->getDataInfo();

    return nullptr;
}

std::vector<IODataType> CWorkflow::getRootTargetTypes() const
{
    std::vector<IODataType> types;
    auto outEdges = boost::out_edges(m_root, m_graph);

    for(auto it=outEdges.first; it!=outEdges.second; ++it)
    {
        WorkflowVertex target = boost::target(*it, m_graph);
        WorkflowTaskPtr targetTask = m_graph[target];
        WorkflowEdgePtr edge = m_graph[*it];
        IODataType type = targetTask->getOriginalInputDataType(edge->getTargetIndex());
        types.push_back(type);
    }
    return types;
}

bool CWorkflow::isRoot(const WorkflowVertex &id) const
{
    return id == m_root;
}

bool CWorkflow::isModified() const
{
    uint newHashValue = getHashValue();
    return newHashValue != m_hashValue;
}

bool CWorkflow::isStopped() const
{
    return m_bStopped;
}

bool CWorkflow::isInputConnected(size_t index) const
{
    auto outEdges = boost::out_edges(m_root, m_graph);
    for(auto it=outEdges.first; it!=outEdges.second; ++it)
    {
        WorkflowEdgePtr edge = m_graph[*it];
        if(edge->getSourceIndex() == index)
            return true;
    }
    return false;
}

bool CWorkflow::isConnectedToRoot(const WorkflowVertex &id) const
{
    auto parents = getParents(id);
    auto result = std::find(parents.begin(), parents.end(), m_root);
    return result != parents.end();
}

bool CWorkflow::isValid(const WorkflowVertex& id)
{
    auto vds = getVertices();
    return std::count(vds.first, vds.second, id) != 0;
}

bool CWorkflow::isLeafTask(const WorkflowVertex &id) const
{
    auto outEdgesIt = boost::out_edges(id, m_graph);
    return outEdgesIt.first == outEdgesIt.second;
}

bool CWorkflow::isBatchMode() const
{
    return m_bBatchMode;
}

/***********/
/* SLOTS */
/***********/

void CWorkflow::onTaskInputRemoved(size_t index)
{
    auto pSignalHandler = qobject_cast<CWorkflowSignalHandler*>(m_signalHandler.get());
    auto rangeIt = boost::in_edges(m_activeTask, m_graph);

    for(auto it=rangeIt.first; it!=rangeIt.second;)
    {
        auto edgePtr = m_graph[*it];
        if(edgePtr->getTargetIndex() == index)
        {
            auto itToDelete = it++;
            emit pSignalHandler->doDeleteConnection(*itToDelete);
            deleteEdge(*itToDelete);
        }
        else
            ++it;
    }
}

void CWorkflow::onTaskOutputRemoved(size_t index)
{
    deleteOutEdges(m_activeTask, index);
}

/***********/
/* METHODS */
/***********/

void CWorkflow::addInput(const WorkflowTaskIOPtr& pInput)
{
    CWorkflowTask::addInput(pInput);
    auto pRootTask = m_graph[m_root];
    pRootTask->addInput(pInput);
    pRootTask->addOutput(pInput);
}

void CWorkflow::addInput(const WorkflowTaskIOPtr&& pInput)
{
    CWorkflowTask::addInput(pInput);
    auto pRootTask = m_graph[m_root];
    pRootTask->addInput(pInput);
    pRootTask->addOutput(pInput);
}

void CWorkflow::addInputs(const InputOutputVect &inputs)
{
    CWorkflowTask::addInputs(inputs);
    auto pRootTask = m_graph[m_root];
    pRootTask->addInputs(inputs);
    pRootTask->addOutputs(inputs);
}

void CWorkflow::removeInput(size_t index)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    CWorkflowTask::removeInput(index);
    auto pRootTask = m_graph[m_root];
    pRootTask->removeInput(index);
    pRootTask->removeOutput(index);
    decrementOutEdgesSrcIndex(m_root, index);
}

WorkflowVertex CWorkflow::addTask(const WorkflowTaskPtr& pNewTask)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    assert(pNewTask != nullptr);
    connectSignals(pNewTask);
    pNewTask->setOutputFolder(m_outputFolder);
    pNewTask->setGraphicsContext(m_graphicsContextPtr);
    pNewTask->setAutoSave(m_bAutoSave);
    m_lastTaskAdded = boost::add_vertex(pNewTask, m_graph);
    return m_lastTaskAdded;
}

void CWorkflow::replaceTask(const WorkflowTaskPtr &pNewTask, const WorkflowVertex &id)
{
    assert(pNewTask != nullptr);
    std::lock_guard<std::mutex> lock(m_mutex);

    if(id != boost::graph_traits<WorkflowGraph>::null_vertex())
    {
        connectSignals(pNewTask);
        m_graph[id] = pNewTask;
        pNewTask->setOutputFolder(m_outputFolder);
        pNewTask->setAutoSave(m_bAutoSave);
    }
}

std::vector<WorkflowEdge> CWorkflow::connect(const WorkflowVertex &src, const WorkflowVertex &target)
{
    WorkflowVertex source = src;
    if(source == boost::graph_traits<WorkflowGraph>::null_vertex())
        source = m_root;

    std::vector<WorkflowEdge> connections;
    auto ports = findConnectionPorts(source, target);

    if(ports.empty() == false)
    {
        for(size_t i=0; i<ports.size(); ++i)
        {
            if(ports[i].first != SIZE_MAX && ports[i].second != SIZE_MAX)
            {
                auto connection = connect(source, ports[i].first, target, ports[i].second);
                connections.push_back(connection);
            }
        }
    }
    else
    {
        WorkflowTaskPtr taskPtr = getTask(source);
        if(source != m_root || taskPtr->getInputCount() > 0)
            throw CException(CoreExCode::INVALID_CONNECTION, QObject::tr("No valid connection available, please check input/output data types.").toStdString(), __func__, __FILE__, __LINE__);
        else
        {
            // Connect to root port 0
            auto connection = connect(source, 0, target, 0);
            connections.push_back(connection);
        }
    }
    return connections;
}

WorkflowEdge CWorkflow::connect(const WorkflowVertex &src, size_t srcIndex, const WorkflowVertex &target, size_t targetIndex)
{
    std::pair<WorkflowEdge, bool> returnPair;
    auto pEdge = std::make_shared<CWorkflowEdge>(srcIndex, targetIndex);

    WorkflowVertex source = src;
    if(source == boost::graph_traits<WorkflowGraph>::null_vertex())
        source = m_root;

    //Get task
    auto srcTaskPtr = m_graph[source];
    auto targetTaskPtr = m_graph[target];

    if(source == m_root && srcTaskPtr->getOutputCount() == 0)
    {
        //Add connection
        returnPair = boost::add_edge(source, target, pEdge, m_graph);
    }
    else
    {
        if(checkConnection(source, srcIndex, target, targetIndex) == false)
        {
            QString errorMsg = QObject::tr("Invalid connection between output #%1 of %2 (%3) and input #%4 of %5 (%6)")
                                        .arg(srcIndex+1)
                                        .arg(QString::fromStdString(srcTaskPtr->getName()))
                                        .arg(Utils::Workflow::getIODataName(srcTaskPtr->getOutputDataType(srcIndex)))
                                        .arg(targetIndex+1)
                                        .arg(QString::fromStdString(targetTaskPtr->getName()))
                                        .arg(Utils::Workflow::getIODataName(targetTaskPtr->getInputDataType(targetIndex)));
            throw CException(CoreExCode::INVALID_PARAMETER, errorMsg.toStdString(), __func__, __FILE__, __LINE__);
        }

        //Add connection
        returnPair = boost::add_edge(source, target, pEdge, m_graph);

        if(source == m_root)
            checkBatchModeState();

        //Forward output of source task to input of target task
        targetTaskPtr->setInput(srcTaskPtr->getOutput(srcIndex), targetIndex);
        startIOAnalysis(target);

        //Check graphics input -> automatically set the current graphics layer as graphics input of the task
        //Add "&& pGraphicsInput->isDataAvailable() == true" to invert the behavior
        auto pGraphicsInput = getGraphicsInput(m_graph[target]);
        auto pSignalHandler = static_cast<CWorkflowSignalHandler*>(getSignalRawPtr());

        if(pGraphicsInput != nullptr)
            emit pSignalHandler->doUpdateTaskGraphicsInput(pGraphicsInput);
    }
    return returnPair.first;
}

void CWorkflow::deleteTask(const WorkflowVertex &id)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(id == m_root)
        return;

    CPyEnsureGIL gil;
    boost::clear_vertex(id, m_graph);    
    boost::remove_vertex(id, m_graph);

    if(id == m_activeTask)
        m_activeTask = m_root;
    if(id == m_lastTaskAdded)
        m_lastTaskAdded = m_root;
    if(id == m_runningTask)
        m_runningTask = boost::graph_traits<WorkflowGraph>::null_vertex();
}

void CWorkflow::deleteEdge(const WorkflowEdge &id)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto index = getEdge(id)->getTargetIndex();
    auto src = getEdgeSource(id);
    auto target = getEdgeTarget(id);
    auto pTask = getTask(target);
    resetTaskInput(pTask, index);
    pTask->clearOutputData();
    boost::remove_edge(id, m_graph);

    if(src == m_root)
        checkBatchModeState();

    startIOAnalysis(target);

    auto pSignalHandler = qobject_cast<CWorkflowSignalHandler*>(m_signalHandler.get());
    //Change task item status to error (connection missing)
    emit pSignalHandler->doSetTaskState(target, CWorkflowTask::State::_ERROR, QObject::tr("One connection has been removed"));
}

void CWorkflow::deleteOutEdges(const WorkflowVertex &taskId, size_t srcIndex)
{
    auto pSignalHandler = qobject_cast<CWorkflowSignalHandler*>(m_signalHandler.get());
    auto outEdges = boost::out_edges(taskId, m_graph);

    for(auto it=outEdges.first; it!=outEdges.second;)
    {
        auto edgePtr = m_graph[*it];
        if(edgePtr->getSourceIndex() == srcIndex)
        {
            auto itToDelete = it++;
            emit pSignalHandler->doDeleteConnection(*itToDelete);
            deleteEdge(*itToDelete);
        }
        else
            ++it;
    }
}

void CWorkflow::clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_graph.clear();
}

void CWorkflow::clearInputs()
{
    CWorkflowTask::clearInputs();

    auto pRootTask = m_graph[m_root];
    if(pRootTask)
    {
        pRootTask->clearInputs();
        pRootTask->clearOutputs();
    }
}

void CWorkflow::clearAllOutputData()
{
    auto vertexRangeIt = boost::vertices(m_graph);
    for(auto it=vertexRangeIt.first; it!=vertexRangeIt.second; ++it)
    {
        WorkflowTaskPtr taskPtr = m_graph[*it];
        if(*it != m_root && taskPtr)
            taskPtr->clearOutputData();
    }
}

void CWorkflow::forceBatchMode(bool bEnable)
{
    m_bBatchMode = bEnable;
    m_bForceBatchMode = bEnable;
    m_runMgr.setBatchMode(bEnable);
}

void CWorkflow::run()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(getTaskCount() == 0)
        throw CException(CoreExCode::INVALID_SIZE, "Empty workflow, no task to run", __func__, __FILE__, __LINE__);

    //Traverse graph and run each task
    clearOutputs();
    auto tasks = getForwardPassTasks(m_root);
    updateCompositeInputName();

    for(size_t i=0; i<tasks.size(); ++i)
        runTask(tasks[i]);        

    emit m_signalHandler->doFinishWorkflow();
}

void CWorkflow::runFrom(const WorkflowVertex &id)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(getTaskCount() == 0)
        throw CException(CoreExCode::INVALID_SIZE, "Empty workflow can't be run", __func__, __FILE__, __LINE__);

    auto tasks = getForwardPassTasks(id);

    // Search for and run all task not already executed before id
    bool flag = m_graph[id]->getOutputs().empty();
    if(flag == true)
    {
        std::vector<WorkflowVertex> taskToExecute;
        findTaskToExecute(taskToExecute, id);

        if(taskToExecute.empty() == false)
            runNeededTask(taskToExecute);
        else
        {
            auto pTask = m_graph[id];
            if(pTask == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Null pointer exception", __func__, __FILE__, __LINE__);

            //Fill inputs of current task
            auto inEdgesIt = boost::in_edges(id, m_graph);
            for(auto it=inEdgesIt.first; it!=inEdgesIt.second; ++it)
            {
                auto srcVertex = boost::source(*it, m_graph);
                auto pSrcTask = m_graph[srcVertex];
                auto pEdge = m_graph[*it];

                if(pSrcTask)
                    pTask->setInput(pSrcTask->getOutput(pEdge->getSourceIndex()), pEdge->getTargetIndex());
            }
        }
    }

    //Traverse graph and run each task
    Utils::print("Workflow started", QtMsgType::QtDebugMsg);
    clearOutputs();
    updateCompositeInputName();

    for(size_t i=0; i<tasks.size(); ++i)
        runTask(tasks[i]);

    emit m_signalHandler->doFinishWorkflow();
}

void CWorkflow::runLastTask()
{
    runTo(m_lastTaskAdded);
}

void CWorkflow::runNeededTask(const std::vector<WorkflowVertex>& taskToExecute)
{
    for(auto vertexIt=taskToExecute.begin(); vertexIt!=taskToExecute.end() && m_bStopped == false; ++vertexIt)
    {
        auto pTask = m_graph[*vertexIt];
        if(pTask == nullptr)
            throw CException(CoreExCode::INVALID_PARAMETER, "Null pointer exception", __func__, __FILE__, __LINE__);
        else
        {
            //Fill inputs
            auto inEdgesIt = boost::in_edges(*vertexIt, m_graph);
            for(auto it=inEdgesIt.first; it!=inEdgesIt.second; ++it)
            {
                auto srcVertex = boost::source(*it, m_graph);
                auto pSrcTask = m_graph[srcVertex];
                auto pEdge = m_graph[*it];

                if(pSrcTask)
                    pTask->setInput(pSrcTask->getOutput(pEdge->getSourceIndex()), pEdge->getTargetIndex());
            }
            runTask(*vertexIt);
        }
    }

    if(m_bStopped == true)
    {
        m_bStopped = false;
        throw CException(CoreExCode::PROCESS_CANCELLED, "Stop workflow requested", __func__, __FILE__, __LINE__);
    }
}

void CWorkflow::runTo(const WorkflowVertex& id)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(getTaskCount() == 0)
        throw CException(CoreExCode::INVALID_SIZE, "Empty workflow can't be run", __func__, __FILE__, __LINE__);

    // Search for and run all task not already executed before id
    std::vector<WorkflowVertex> taskToExecute;
    taskToExecute.push_back(id);
    findTaskToExecute(taskToExecute, id);
    updateCompositeInputName();
    // Run tasks
    runNeededTask(taskToExecute);
    emit m_signalHandler->doFinishWorkflow();
}

void CWorkflow::runTask(const WorkflowVertex& id)
{
    auto taskPtr = m_graph[id];
    if(taskPtr == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Null pointer exception", __func__, __FILE__, __LINE__);
    else
    {
        if(m_bStopped == true)
        {
            m_bStopped = false;
            throw CException(CoreExCode::PROCESS_CANCELLED, QObject::tr("Workflow stop requested by user").toStdString(), __func__, __FILE__, __LINE__);
        }

        // Update output folder
        auto baseFolder = taskPtr->getOutputFolder();
        taskPtr->setOutputFolder(baseFolder + m_startDate + "/" + taskPtr->getName() + "/");
        // Run task
        setRunningTask(id);
        m_runMgr.run(taskPtr, m_compositeInputName);
        manageOutputs(id);
        // Restore output folder
        taskPtr->setOutputFolder(baseFolder);

        auto pSignalHandler = static_cast<CWorkflowSignalHandler*>(m_signalHandler.get());
        emit pSignalHandler->doFinishTask(id, CWorkflowTask::State::VALIDATE);
    }
}

void CWorkflow::startIOAnalysis(const WorkflowVertex& idFrom)
{
    //If workflow is empty -> only root task
    if(getTaskCount() == 1)
        return;

    auto tasks = getForwardPassTasks(idFrom);
    for(size_t i=0; i<tasks.size(); ++i)
        analyzeTaskIO(tasks[i]);
}

void CWorkflow::startDepthFirstSearch()
{
    //Create property map of unique indices for bfs algorithm
    VertexIndexMap mapIndex;
    auto propMapIndex = createBfsPropertyMap(mapIndex);

    CGenericDfsVisitor visitor([this](const WorkflowVertex& id)
                                {
                                    auto pSignalHandler = static_cast<CWorkflowSignalHandler*>(getSignalRawPtr());
                                    emit pSignalHandler->doDfsTaskVisited(id);
                                });
    boost::depth_first_search(m_graph, boost::visitor(visitor).vertex_index_map(propMapIndex));
}

void CWorkflow::stop()
{
    m_bStopped = true;
    m_signalHandler->doSetMessage(QObject::tr("Stopping workflow..."));
    m_runMgr.stop(getTask(m_runningTask));
}

void CWorkflow::analyzeTaskIO(const WorkflowVertex &id)
{
    using InEdgeIt = boost::graph_traits<WorkflowGraph>::in_edge_iterator;

    auto taskPtr = m_graph[id];
    if(taskPtr == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Null pointer exception", __func__, __FILE__, __LINE__);
    else
    {
        auto pSignalHandler = static_cast<CWorkflowSignalHandler*>(getSignalRawPtr());
        auto inEdgesIt = boost::in_edges(id, m_graph);

        //Check connections validity
        for(auto it=inEdgesIt.first; it!=inEdgesIt.second;)
        {
            auto pEdge = m_graph[*it];
            auto srcTaskId = boost::source(*it, m_graph);
            auto srcTaskPtr = m_graph[srcTaskId];
            auto srcIndex = pEdge->getSourceIndex();
            auto targetIndex = pEdge->getTargetIndex();

            if(srcIndex >= srcTaskPtr->getOutputCount())
            {
                //Invalid connection -> input does not exist, we have to reset input
                resetTaskInput(taskPtr, targetIndex);
                //Notify view to update task status to error
                QString errorMsg = QObject::tr("No possible connection to input #%1").arg(pEdge->getTargetIndex()+1);
                emit pSignalHandler->doSetTaskState(id, CWorkflowTask::State::_ERROR, errorMsg);

                //Delete edge if source task is not root
                if(isRoot(srcTaskId) == false)
                {
                    InEdgeIt itToDelete = it;
                    it++;
                    deleteEdge(*itToDelete);
                }
                else
                    it++;
            }
            else
            {
                //Forward output
                taskPtr->setInput(srcTaskPtr->getOutput(srcIndex), targetIndex);
                if(Utils::Workflow::isIODataCompatible(srcTaskPtr->getOutputDataType(srcIndex), taskPtr->getInputDataType(targetIndex)) == false)
                {
                    //Invalid connection -> type mismatch, we have to reset input
                    resetTaskInput(taskPtr, targetIndex);
                    //Notify view to update task status to error
                    QString errorMsg = QObject::tr("Data type mismatch on input #%1 between type %2 and type %3")
                                                    .arg(pEdge->getTargetIndex()+1)
                                                    .arg(Utils::Workflow::getIODataName(srcTaskPtr->getOutputDataType(pEdge->getSourceIndex())))
                                                    .arg(Utils::Workflow::getIODataName(taskPtr->getInputDataType(pEdge->getTargetIndex())));
                    emit pSignalHandler->doSetTaskState(id, CWorkflowTask::State::_ERROR, errorMsg);
                }
                it++;
            }
        }

        //Check graphics input -> automatically set the current graphics layer as graphics input of the task
        auto graphicsInputPtr = getGraphicsInput(m_graph[id]);
        if(graphicsInputPtr != nullptr)
        {
            auto pSignalHandler = static_cast<CWorkflowSignalHandler*>(getSignalRawPtr());
            emit pSignalHandler->doUpdateTaskGraphicsInput(graphicsInputPtr);
        }

        // Update batch input flag
        taskPtr->setBatchInput(m_bBatchMode);

        //Notify view to update task item (IO ports - actions), connections in view are automatically deleted here
        emit pSignalHandler->doUpdateTaskItemView(taskPtr, id);
    }
}

void CWorkflow::createRoot()
{
    auto pRootTask = std::make_shared<CWorkflowTask>("Root");
    pRootTask->setInputs(getInputs(), false);
    m_root = boost::add_vertex(pRootTask, m_graph);
    m_lastTaskAdded = m_root;
}

BfsPropertyMap CWorkflow::createBfsPropertyMap(VertexIndexMap& mapIndex) const
{
    boost::associative_property_map<VertexIndexMap> propMapIndex(mapIndex);
    size_t i=0;

    WorkflowGraph::vertex_iterator vi, vi_end;
    for(boost::tie(vi, vi_end) = boost::vertices(m_graph); vi != vi_end; ++vi)
        boost::put(propMapIndex, *vi, i++);

    return propMapIndex;
}

bool CWorkflow::checkConnection(const WorkflowVertex &src, size_t srcIndex, const WorkflowVertex &target, size_t targetIndex)
{
    //Loop forbidden
    if(src == target)
        return false;

    auto pSrcTask = m_graph[src];
    auto pTargetTask = m_graph[target];

    if(pSrcTask == nullptr || pTargetTask == nullptr)
        return false;

    if(srcIndex >= pSrcTask->getOutputCount())
        return false;

    if(targetIndex >= pTargetTask->getInputCount())
        return false;

    return Utils::Workflow::isIODataCompatible(pSrcTask->getOutputDataType(srcIndex), pTargetTask->getOriginalInputDataType(targetIndex));
}

void CWorkflow::checkBatchModeState()
{
    m_bBatchMode = false;
    for(size_t i=0; i<m_inputBatchState.size(); ++i)
    {
        if(m_inputBatchState[i] == true)
        {
            auto edges = boost::out_edges(m_root, m_graph);
            for(auto it=edges.first; it!=edges.second; ++it)
            {
                WorkflowEdgePtr edge = m_graph[*it];
                if(edge->getSourceIndex() == i)
                {
                    m_bBatchMode = true;
                    break;
                }
            }
        }
    }
    m_runMgr.setBatchMode(m_bBatchMode);
}

void CWorkflow::updateHash()
{
    m_hashValue = getHashValue();
}

void CWorkflow::updateStartTime()
{
    m_startDate = Utils::File::conformName(QDateTime::currentDateTime().toString(Qt::ISODate)).toStdString();
}

void CWorkflow::connectSignals(const WorkflowTaskPtr& pNewTask)
{
    // Connect new task signals for progress bar
    // Don't forget to connect in CWorkflowManager::createWorkflow !!
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doProgress, this->getSignalRawPtr(), &CSignalHandler::onProgress);
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doSetMessage, this->getSignalRawPtr(), &CSignalHandler::onSetMessage);
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doSetElapsedTime, this->getSignalRawPtr(), &CSignalHandler::onSetElapsedTime);
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doAddSubTotalSteps, this->getSignalRawPtr(), &CSignalHandler::onAddSubTotalSteps);
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doAddGraphicsLayer, this->getSignalRawPtr(), &CSignalHandler::onAddGraphicsLayer);
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doRemoveGraphicsLayer, this->getSignalRawPtr(), &CSignalHandler::onRemoveGraphicsLayer);
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doOutputChanged, this->getSignalRawPtr(), &CSignalHandler::onOutputChanged);
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doInputRemoved, [this](size_t index){ this->onTaskInputRemoved(index); });
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doOutputRemoved, [this](size_t index){ this->onTaskOutputRemoved(index); });
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doGraphicsContextChanged, this->getSignalRawPtr(), &CSignalHandler::onGraphicsContextChanged);
    QObject::connect(pNewTask->getSignalRawPtr(), &CSignalHandler::doLog, [&](const QString& msg)
    {
        if(Utils::IkomiaApp::isStarted())
            getSignalRawPtr()->doLog(msg);
        else
            Utils::print(msg, QtMsgType::QtInfoMsg);
    });
}

void CWorkflow::findTaskToExecute(std::vector<WorkflowVertex>& list, const WorkflowVertex& id) const
{
    if(id == m_root)
        return;

    auto parents = getParents(id);
    for(auto& it : parents)
    {
       if(m_graph[it]->hasOutputData() == false)
       {
           //If task already exists, we have to remove it and insert it to front
           auto itFind = std::find(list.begin(), list.end(), it);
           if(itFind != list.end())
               list.erase(itFind);

           list.insert(list.begin(), it);
           findTaskToExecute(list, it);
       }
    }
}

void CWorkflow::findPreviousTasks(std::vector<WorkflowVertex>& list, const WorkflowVertex& id)
{
    if(id == m_root)
        return;

    auto parents = getParents(id);
    for(auto& it : parents)
    {
       if(m_graph[it]->hasOutputData() == true)
       {
           //If task already exists, we have to remove it and insert it to front
           auto itFind = std::find(list.begin(), list.end(), it);
           if(itFind != list.end())
               list.erase(itFind);

           list.insert(list.begin(), it);
           findPreviousTasks(list, it);
       }
    }
}

std::vector<std::pair<size_t, size_t>> CWorkflow::findConnectionPorts(const WorkflowVertex &srcId, const WorkflowVertex &dstId)
{
    std::vector<std::pair<size_t, size_t>> ports;
    std::unordered_map<size_t, size_t> portConnectionCount;

    auto srcTaskPtr = m_graph[srcId];
    auto targetTaskPtr = m_graph[dstId];

    if(srcTaskPtr && targetTaskPtr)
    {
        auto outEdgesIt = boost::out_edges(srcId, m_graph);
        for(auto it=outEdgesIt.first; it!=outEdgesIt.second; ++it)
        {
            auto edgePtr = m_graph[*it];
            size_t portIndex = edgePtr->getSourceIndex();
            auto itPort = portConnectionCount.find(portIndex);

            if(itPort == portConnectionCount.end())
                portConnectionCount.insert(std::make_pair(portIndex, 1));
            else
                itPort->second++;
        }

        for(size_t i=0; i<targetTaskPtr->getInputCount(); ++i)
        {
            size_t portIndexFound = SIZE_MAX;
            size_t minConnectionCount = SIZE_MAX;
            std::vector<size_t> candidates;
            auto targetDataType = targetTaskPtr->getInputDataType(i);

            //Find candidates source ports
            for(size_t j=0; j<srcTaskPtr->getOutputCount(); ++j)
            {
                auto srcDataType = srcTaskPtr->getOutputDataType(j);
                if(Utils::Workflow::isIODataCompatible(srcDataType, targetDataType))
                    candidates.push_back(j);
            }

            //Choose port that minimizes connection count
            for(size_t j=0; j<candidates.size(); ++j)
            {
                auto it = portConnectionCount.find(candidates[j]);
                if(it == portConnectionCount.end())
                {
                    portIndexFound = candidates[j];
                    break;
                }
                else
                {
                    if(it->second < minConnectionCount)
                    {
                        portIndexFound = candidates[j];
                        minConnectionCount = it->second;
                    }
                }
            }

            if(portIndexFound != SIZE_MAX)
            {
                //Add connection ports
                ports.push_back(std::make_pair(portIndexFound, i));

                //Update structure storing connection count
                auto it = portConnectionCount.find(portIndexFound);
                if(it == portConnectionCount.end())
                    portConnectionCount.insert(std::make_pair(portIndexFound, 1));
                else
                    it->second++;
            }
        }
    }
    return ports;
}

void CWorkflow::resetTaskInput(WorkflowTaskPtr &taskPtr, size_t index)
{
    if(m_pTaskIORegistration)
    {
        auto dataType = taskPtr->getOriginalInputDataType(index);
        auto factory = m_pTaskIORegistration->getFactory();
        auto taskIOPtr = factory.createObject(CWorkflowTaskIO::getClassName(dataType), std::move(dataType));

        if(taskIOPtr)
            taskPtr->resetInput(index, taskIOPtr);
    }
    else
        taskPtr->clearInputData(index);
}

void CWorkflow::decrementOutEdgesSrcIndex(const WorkflowVertex &taskId, size_t indexFrom)
{
    auto outEdges = boost::out_edges(taskId, m_graph);
    for(auto it=outEdges.first; it!=outEdges.second; ++it)
    {
        auto edgePtr = m_graph[*it];
        size_t srcIndex = edgePtr->getSourceIndex();

        if(srcIndex > indexFrom)
            edgePtr->setSourceIndex(srcIndex - 1);
    }
}

void CWorkflow::updateCompositeInputName()
{
    m_compositeInputName.clear();
    WorkflowTaskPtr rootTaskPtr = m_graph[m_root];
    auto edgeRange = getOutEdges(m_root);
    std::map<int, std::string> inputNames;

    for(auto it=edgeRange.first; it!=edgeRange.second; ++it)
    {
        WorkflowEdgePtr edge = m_graph[*it];
        int sourceIndex = edge->getSourceIndex();
        auto inputPtr = rootTaskPtr->getInput(sourceIndex);

        if(inputPtr && inputNames.find(sourceIndex) == inputNames.end())
        {
            std::string name = inputPtr->getName();
            if(name.empty())
            {
                CDataInfoPtr infoPtr = inputPtr->getDataInfo();
                if(infoPtr)
                {
                    std::string basePath = Utils::File::getPathFromPattern(infoPtr->getFileName(), 0);
                    name = Utils::File::getFileNameWithoutExtension(basePath);
                }
            }

            if(name.empty())
                inputNames[sourceIndex] = "empty";
            else
                inputNames[sourceIndex] = name;
        }
    }

    for(auto it=inputNames.begin(); it!=inputNames.end(); ++it)
    {
        if(!m_compositeInputName.empty())
            m_compositeInputName += "-";

        m_compositeInputName += it->second;
    }
}

void CWorkflow::save(const std::string &path)
{
    auto ext = Utils::File::extension(path);
    if(ext == ".json")
        saveJSON(path);
    else
        throw CException(CoreExCode::NOT_IMPLEMENTED, "Workflow can only be saved as JSON file", __func__, __FILE__, __LINE__);
}

void CWorkflow::load(const std::string &path)
{
    auto ext = Utils::File::extension(path);
    if(ext == ".json")
        loadJSON(path);
    else
        throw CException(CoreExCode::NOT_IMPLEMENTED, "Workflow can only be loaded as JSON file", __func__, __FILE__, __LINE__);
}

void CWorkflow::saveJSON(const std::string& path)
{
    QFile jsonFile(QString::fromStdString(path));
    if(!jsonFile.open(QFile::WriteOnly))
        throw CException(CoreExCode::INVALID_FILE, "Could not save file: " + path, __func__, __FILE__, __LINE__);

    int id = 0;
    std::unordered_map<WorkflowVertex, int> mapVertexToId;
    QJsonObject jsonWorkflow;
    QJsonArray jsonTasks, jsonEdges;

    // Metadata
    QJsonObject jsonMetadata;
    jsonMetadata["name"] = QString::fromStdString(m_name);
    jsonMetadata["description"] = QString::fromStdString(m_description);
    jsonMetadata["keywords"] = QString::fromStdString(m_keywords);
    jsonWorkflow["metadata"] = jsonMetadata;

    // Tasks
    auto vertexRangeIt = boost::vertices(m_graph);
    for(auto it=vertexRangeIt.first; it!=vertexRangeIt.second; ++it)
    {
        if(*it != m_root)
        {
            QJsonObject jsonTask;
            jsonTask["task_id"] = id;
            WorkflowTaskPtr taskPtr = m_graph[*it];
            jsonTask["task_data"] = taskPtr->toJson();
            jsonTasks.append(jsonTask);
            mapVertexToId.insert(std::make_pair(*it, id++));
        }
    }
    jsonWorkflow["tasks"] = jsonTasks;

    // Edges
    auto edgeRangeIt = boost::edges(m_graph);
    for(auto it=edgeRangeIt.first; it!=edgeRangeIt.second; ++it)
    {
        QJsonObject jsonEdge;
        WorkflowEdgePtr edgePtr = m_graph[*it];

        // Edge source
        auto itSrcId = mapVertexToId.find(boost::source(*it, m_graph));
        if(itSrcId != mapVertexToId.end())
            jsonEdge["source_id"] = itSrcId->second;
        else
            jsonEdge["source_id"] = -1;

        jsonEdge["source_index"] = (int)edgePtr->getSourceIndex();

        // Edge target
        auto itTargetId = mapVertexToId.find(boost::target(*it, m_graph));
        if(itTargetId != mapVertexToId.end())
            jsonEdge["target_id"] = itTargetId->second;
        else
            jsonEdge["target_id"] = -1;

        jsonEdge["target_index"] = (int)edgePtr->getTargetIndex();
        jsonEdges.append(jsonEdge);
    }
    jsonWorkflow["connections"] = jsonEdges;

    QJsonDocument jsonDoc(jsonWorkflow);
    jsonFile.write(jsonDoc.toJson());
}

void CWorkflow::loadJSON(const std::string &path)
{
    assert(m_pTaskRegistration);
    std::unordered_map<int, WorkflowVertex> mapIdToVertexId;

    QFile jsonFile(QString::fromStdString(path));
    if(!jsonFile.open(QFile::ReadOnly))
        throw CException(CoreExCode::INVALID_FILE, "Could not load file: " + path, __func__, __FILE__, __LINE__);

    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonFile.readAll()));
    if(jsonDoc.isNull() || jsonDoc.isEmpty())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Error while loading workflow: invalid JSON structure", __func__, __FILE__, __LINE__);

    QJsonObject jsonWorkflow = jsonDoc.object();
    if(jsonWorkflow.isEmpty())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Error while loading workflow: empty JSON workflow", __func__, __FILE__, __LINE__);

    // Metadata
    QJsonObject jsonMetadata = jsonWorkflow["metadata"].toObject();
    if(!jsonMetadata.isEmpty())
    {
        m_name = jsonMetadata["name"].toString().toStdString();
        m_description = jsonMetadata["description"].toString().toStdString();
        m_keywords = jsonMetadata["keywords"].toString().toStdString();
    }

    // Load tasks
    QJsonArray jsonTasks = jsonWorkflow["tasks"].toArray();
    for(int i=0; i<jsonTasks.size(); ++i)
    {
        QJsonObject jsonTask = jsonTasks[i].toObject();
        QJsonObject jsonTaskData = jsonTask["task_data"].toObject();
        auto taskPtr = m_pTaskRegistration->createProcessObject(jsonTaskData["name"].toString().toStdString(), nullptr);

        if(taskPtr)
        {
            UMapString paramMap;
            QJsonArray jsonParams = jsonTaskData["parameters"].toArray();

            for(int j=0; j<jsonParams.size(); ++j)
            {
                QJsonObject jsonParam = jsonParams[j].toObject();
                paramMap[jsonParam["name"].toString().toStdString()] = jsonParam["value"].toString().toStdString();
            }
            taskPtr->setParamValues(paramMap);
            taskPtr->parametersModified();
            auto vertexId = addTask(taskPtr);
            mapIdToVertexId.insert(std::make_pair(jsonTask["task_id"].toInt(), vertexId));
        }
    }

    // Load connections
    QJsonArray jsonEdges = jsonWorkflow["connections"].toArray();
    for(int i=0; i<jsonEdges.size(); ++i)
    {
        QJsonObject jsonEdge = jsonEdges[i].toObject();
        int srcId = jsonEdge["source_id"].toInt();
        WorkflowVertex srcTaskId = boost::graph_traits<WorkflowGraph>::null_vertex();

        auto itSrc = mapIdToVertexId.find(srcId);
        if(itSrc != mapIdToVertexId.end())
            srcTaskId = itSrc->second;

        int targetId = jsonEdge["target_id"].toInt();
        WorkflowVertex targetTaskId = boost::graph_traits<WorkflowGraph>::null_vertex();

        auto itTarget = mapIdToVertexId.find(targetId);
        if(itTarget != mapIdToVertexId.end())
            targetTaskId = itTarget->second;

        connect(srcTaskId, jsonEdge["source_index"].toInt(), targetTaskId, jsonEdge["target_index"].toInt());
    }
}

void CWorkflow::manageOutputs(const WorkflowVertex& taskId)
{
    auto pTask = m_graph[taskId];
    if(pTask == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Null pointer exception", __func__, __FILE__, __LINE__);

    auto outEdgesIt = boost::out_edges(taskId, m_graph);
    if(outEdgesIt.first == outEdgesIt.second)
    {
        //Leaf task
        addOutputs(pTask->getOutputs());
    }
    else
    {
        for(auto it=outEdgesIt.first; it!=outEdgesIt.second; ++it)
        {
            auto targetVertex = boost::target(*it, m_graph);
            auto pTargetTask = m_graph[targetVertex];
            auto pEdge = m_graph[*it];

            if(pTargetTask != nullptr && pEdge != nullptr)
                pTargetTask->setInput(pTask->getOutput(pEdge->getSourceIndex()), pEdge->getTargetIndex());
        }
    }

    // Auto-save outputs if in batch mode
    if(m_bBatchMode || m_bAutoSave)
        pTask->saveOutputs(m_compositeInputName);
}

//-------------------//
//- GraphViz export -//
//-------------------//
void CWorkflow::writeGraphviz(const std::string &path)
{
    try
    {
        std::ofstream dotFile(path);
        boost::dynamic_properties dp;
        VertexIndexMap indexMap;
        auto indexPropMap = createBfsPropertyMap(indexMap);
        auto bundle = boost::get(boost::vertex_bundle, m_graph);
        dp.property("node_id", indexPropMap);
        dp.property("label", boost::make_transform_value_property_map(std::mem_fn(&CWorkflowTask::getName), bundle));
        boost::write_graphviz_dp(dotFile, m_graph, dp);
        //Command to convert .dot file to pdf (in terminal): dot -T pdf file.dot -O
    }
    catch(std::exception& e)
    {
        Utils::print(e.what(), QtMsgType::QtCriticalMsg);
    }
}

void CWorkflow::notifyGraphicsChanged()
{
    auto taskPtr = m_graph[m_activeTask];
    if(taskPtr)
    {
        //We consider that the graphicsChanged event proceed a call to run() so we set the number of steps for the progress bar
        emit m_signalHandler->doSetTotalSteps(static_cast<int>(taskPtr->getProgressSteps()));
        taskPtr->graphicsChanged();
        emit m_signalHandler->doFinish();
    }
}

void CWorkflow::notifyVideoStart(int frameCount)
{
    // notify all task
    //Create property map of unique indices for bfs algorithm
    VertexIndexMap mapIndex;
    auto propMapIndex = createBfsPropertyMap(mapIndex);

    CNotifyVideoVisitor visitor(this, frameCount);
    boost::breadth_first_search(m_graph, m_root, boost::visitor(visitor).vertex_index_map(propMapIndex));
}

//---------------------------//
//- Class CFindChildVisitor -//
//---------------------------//
CFindChildVisitor::CFindChildVisitor(std::shared_ptr<std::vector<WorkflowVertex>> pChilds) : boost::default_bfs_visitor()
{
    m_pChilds = pChilds;
}

void CFindChildVisitor::discover_vertex(WorkflowVertex vertexId, const WorkflowGraph &graph)
{
    Q_UNUSED(graph);

    if(m_bFirst == true)
        m_bFirst = false;
    else
        m_pChilds->push_back(vertexId);
}

//------------------------------//
//- Class CProgressStepVisitor -//
//------------------------------//
CProgressStepVisitor::CProgressStepVisitor(size_t unitEltCount, size_t& stepCount) : m_stepcount(stepCount)
{
    m_unitEltCount = unitEltCount;
}

void CProgressStepVisitor::discover_vertex(WorkflowVertex vertexId, const WorkflowGraph &graph)
{
    auto pTask = graph[vertexId];
    if(pTask)
        m_stepcount += pTask->getProgressSteps(m_unitEltCount);
}

//----------------------------//
//- Class CGenericDfsVisitor -//
//----------------------------//
CGenericDfsVisitor::CGenericDfsVisitor(std::function<void (const WorkflowVertex &)> applyToVertex)
{
    m_applyFunc = applyToVertex;
}

void CGenericDfsVisitor::discover_vertex(WorkflowVertex vertexId, const WorkflowGraph &graph)
{
    Q_UNUSED(graph);

    if(m_applyFunc)
        m_applyFunc(vertexId);
}

//----------------------------//
//- Class CNotifyVideoVisitor -//
//----------------------------//
CNotifyVideoVisitor::CNotifyVideoVisitor(CWorkflow* pWorkflow, int frameCount)
{
    m_pWorkflow = pWorkflow;
    m_frameCount = frameCount;
}

void CNotifyVideoVisitor::discover_vertex(WorkflowVertex vertexId, const WorkflowGraph& graph)
{
    auto pTask = graph[vertexId];
    if(pTask)
        pTask->notifyVideoStart(m_frameCount);
}

//----------------------------//
//- Class CElapsedTimeVisitor -//
//----------------------------//
CElapsedTimeVisitor::CElapsedTimeVisitor(double& elapsedTime) : m_elapsedTime(elapsedTime)
{

}

void CElapsedTimeVisitor::discover_vertex(WorkflowVertex vertexId, const WorkflowGraph& graph)
{
    auto pTask = graph[vertexId];
    if(pTask)
        m_elapsedTime += pTask->getElapsedTime();
}

#include "moc_CWorkflow.cpp"
