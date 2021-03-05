/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CPROTOCOL_H
#define CPROTOCOL_H

#ifndef Q_MOC_RUN
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#endif

#include <iostream>
#include "DataProcessGlobal.hpp"
#include "Protocol/CProtocolTask.h"
#include "Protocol/CProtocolEdge.hpp"
#include "Protocol/CProtocolTaskWidget.h"
#include "IO/CTaskIORegistration.h"
#include "IO/CGraphicsProcessInput.h"
#include "CRunTaskManager.h"

Q_DECLARE_LOGGING_CATEGORY(logProtocol)

using ProtocolGraph = boost::adjacency_list<boost::listS,
                                            boost::listS,
                                            boost::bidirectionalS,
                                            std::shared_ptr<CProtocolTask>,
                                            std::shared_ptr<CProtocolEdge>>;

using ProtocolVertex = ProtocolGraph::vertex_descriptor;

using ProtocolEdge = ProtocolGraph::edge_descriptor;

using VertexIndexMap = std::map<ProtocolVertex, size_t>;

using BfsPropertyMap = boost::associative_property_map<VertexIndexMap>;

using ProtocolVertexRangeIt = std::pair<boost::graph_traits<ProtocolGraph>::vertex_iterator,
                                        boost::graph_traits<ProtocolGraph>::vertex_iterator>;

using ProtocolEdgeRangeIt = std::pair<boost::graph_traits<ProtocolGraph>::edge_iterator,
                                      boost::graph_traits<ProtocolGraph>::edge_iterator>;

using ProtocolInEdgeRangeIt = std::pair<boost::graph_traits<ProtocolGraph>::in_edge_iterator,
                                        boost::graph_traits<ProtocolGraph>::in_edge_iterator>;

using ProtocolOutEdgeRangeIt = std::pair<boost::graph_traits<ProtocolGraph>::out_edge_iterator,
                                         boost::graph_traits<ProtocolGraph>::out_edge_iterator>;

class DATAPROCESSSHARED_EXPORT CProtocolSignalHandler : public CSignalHandler
{
    Q_OBJECT

    signals:

        void    doFinishTask(const ProtocolVertex& id, CProtocolTask::State status, const QString& msg = QString());
        void    doUpdateTaskItemView(const ProtocolTaskPtr& taskPtr, const ProtocolVertex& id);
        void    doUpdateTaskGraphicsInput(const GraphicsProcessInputPtr& input);
        void    doSetTaskState(const ProtocolVertex& id, CProtocolTask::State status, const QString& msg);
        void    doDeleteConnection(const ProtocolEdge& id);
        void    doDfsTaskVisited(const ProtocolVertex& id);
};

class DATAPROCESSSHARED_EXPORT CProtocol : public CProtocolTask
{
    public:

        //Constructors
        CProtocol();
        CProtocol(std::string getName);
        CProtocol(const CProtocol& protocol);
        CProtocol(const CProtocol&& protocol);

        //Destructor
        ~CProtocol();

        //Operators
        CProtocol&                      operator=(const CProtocol& protocol);
        CProtocol&                      operator=(const CProtocol&& protocol);
        ProtocolTaskPtr                 operator[](ProtocolVertex v);
        ProtocolEdgePtr                 operator[](ProtocolEdge e);

        //Setters
        void                            setDescription(const std::string& description);
        void                            setKeywords(const std::string& keywords);
        void                            setInput(const ProtocolTaskIOPtr& pInput, size_t index, bool bNewSequence) override;
        void                            setInputs(const InputOutputVect& inputs, bool bNewSequence) override;
        void                            setOutputFolder(const std::string& folder) override;
        void                            setActiveTask(const ProtocolVertex& id);
        void                            setRunningTask(const ProtocolVertex& id);
        void                            setTaskActionFlag(const ProtocolVertex &id, ActionFlag action, bool bEnable);
        void                            setTaskIORegistration(CTaskIORegistration* pRegistration);
        void                            setInputBatchState(size_t index, bool bBatch);

        //Getters
        std::string                     getDescription() const;
        std::string                     getKeywords() const;
        ProtocolVertex                  getRootId() const;
        size_t                          getTaskCount() const;
        size_t                          getChildCount(const ProtocolVertex& parent) const;
        size_t                          getChildIndex(const ProtocolVertex& parent, const ProtocolVertex& child) const;
        ProtocolVertex                  getLastTaskId() const;
        ProtocolVertex                  getActiveTaskId() const;
        ProtocolVertex                  getRunningTaskId() const;
        ProtocolVertex                  getTaskId(const std::string& name) const;
        ProtocolTaskPtr                 getTask(const ProtocolVertex& id);
        std::vector<ProtocolVertex>     getParents(const ProtocolVertex& id) const;
        void                            getAllParents(const ProtocolVertex& id, std::vector<ProtocolVertex>& parents) const;
        std::vector<ProtocolVertex>     getChilds(const ProtocolVertex &parent) const;
        std::vector<ProtocolVertex>     getAllChilds(const ProtocolVertex &parent) const;
        std::vector<ProtocolTaskPtr>    getFinalTasks();
        ProtocolVertexRangeIt           getVertices();
        ProtocolEdgeRangeIt             getEdges();
        ProtocolInEdgeRangeIt           getInEdges(const ProtocolVertex& id);
        size_t                          getInEdgesCount(const ProtocolVertex& id) const;
        ProtocolOutEdgeRangeIt          getOutEdges(const ProtocolVertex& id);
        size_t                          getOutEdgeCount(const ProtocolVertex& id) const;
        ProtocolEdgePtr                 getEdge(const ProtocolEdge& id) const;
        ProtocolVertex                  getEdgeSource(const ProtocolEdge& id);
        ProtocolVertex                  getEdgeTarget(const ProtocolEdge& id);
        uint                            getHashValue() const;
        size_t                          getProgressSteps() override;
        size_t                          getProgressStepsFrom(const ProtocolVertex& idFrom) const;
        size_t                          getProgressStepsTo(const ProtocolVertex& idTo) const;
        GraphicsProcessInputPtr         getGraphicsInput(const ProtocolTaskPtr& pTask);
        double                          getElapsedTimeTo(const ProtocolVertex& id);
        double                          getTotalElapsedTime() const;
        CDataInfoPtr                    getIOInfo(const ProtocolVertex& id, size_t index, bool bInput);

        bool                            isRoot(const ProtocolVertex& id) const;
        bool                            isModified() const;
        bool                            isStopped() const;
        bool                            isInputConnected(size_t index) const;
        bool                            isConnectedToRoot(const ProtocolVertex& id) const;
        bool                            isValid(const ProtocolVertex& id);
        bool                            isLeafTask(const ProtocolVertex& id) const;
        bool                            isBatchMode() const;

        //Methods
        void                            addInput(const ProtocolTaskIOPtr& pInput) override;
        void                            addInput(const ProtocolTaskIOPtr&& pInput) override;
        void                            addInputs(const InputOutputVect &inputs) override;

        void                            removeInput(size_t index) override;

        ProtocolVertex                  addTask(const ProtocolTaskPtr& pNewTask);

        void                            replaceTask(const ProtocolTaskPtr& pNewTask, const ProtocolVertex& id);

        std::vector<ProtocolEdge>       connect(const ProtocolVertex& srcId, const ProtocolVertex& dstId);
        ProtocolEdge                    connect(const ProtocolVertex& srcId, size_t srcIndex, const ProtocolVertex& dstId, size_t dstIndex);

        void                            deleteTask(const ProtocolVertex& id);
        void                            deleteEdge(const ProtocolEdge& id);
        void                            deleteOutEdges(const ProtocolVertex& taskId, size_t srcIndex);

        void                            clear();
        void                            clearInputs() override;
        void                            clearAllOutputData();

        void                            forceBatchMode(bool bEnable);

        void                            run() override;
        void                            runFrom(const ProtocolVertex& id);
        void                            runLastTask();
        void                            runNeededTask(const std::vector<ProtocolVertex>& taskToExecute);
        void                            runTo(const ProtocolVertex& id);
        void                            runTask(const ProtocolVertex &id);

        void                            startIOAnalysis(const ProtocolVertex &idFrom);
        void                            startDepthFirstSearch();

        void                            stop();

        void                            analyzeTaskIO(const ProtocolVertex& id);

        void                            updateHash();

        void                            manageOutputs(const ProtocolVertex &taskId);

        void                            writeGraphviz(const std::string& path);

        void                            notifyGraphicsChanged();

        virtual void                    notifyVideoStart(int frameCount) override;

        // Tells compiler (clang) we want all getProgressSteps functions
        using CProtocolTask::getProgressSteps;

    private:

        size_t                          getProgressSteps(const std::vector<ProtocolVertex>& tasks) const;
        std::vector<ProtocolVertex>     getForwardPassTasks(const ProtocolVertex& startVertex);
        std::vector<ProtocolVertex>     getSelfInputTasks() const;

        void                            createRoot();
        BfsPropertyMap                  createBfsPropertyMap(VertexIndexMap &mapIndex) const;

        bool                            checkConnection(const ProtocolVertex& src, size_t srcIndex, const ProtocolVertex& target, size_t targetIndex);
        void                            checkBatchModeState();

        void                            connectSignals(const ProtocolTaskPtr& pNewTask);

        void                            findTaskToExecute(std::vector<ProtocolVertex>& list, const ProtocolVertex& id) const;
        void                            findPreviousTasks(std::vector<ProtocolVertex>& list, const ProtocolVertex& id);
        std::vector<std::pair<size_t, size_t> > findConnectionPorts(const ProtocolVertex& srcId, const ProtocolVertex& dstId);

        void                            resetTaskInput(ProtocolTaskPtr& taskPtr, size_t index);

        void                            onTaskInputRemoved(size_t index);
        void                            onTaskOutputRemoved(size_t index);

        void                            decrementOutEdgesSrcIndex(const ProtocolVertex &taskId, size_t indexFrom);

        void                            updateCompositeInputName();

    private:

        std::mutex              m_mutex;
        uint                    m_hashValue = 0;
        std::string             m_description;
        std::string             m_keywords;
        std::string             m_compositeInputName;
        ProtocolGraph           m_graph;
        ProtocolVertex          m_root;
        ProtocolVertex          m_lastTaskAdded;
        ProtocolVertex          m_activeTask;
        ProtocolVertex          m_runningTask;
        std::vector<bool>       m_inputBatchState;
        std::atomic<bool>       m_bStopped{false};
        bool                    m_bBatchMode = false;
        bool                    m_bForceBatchMode = false;
        CTaskIORegistration*    m_pTaskIORegistration = nullptr;
        CRunTaskManager         m_runMgr;
};

//---------------------------//
//- Class CFindChildVisitor -//
//---------------------------//
class CFindChildVisitor: public boost::default_bfs_visitor
{
    public:

        CFindChildVisitor(std::shared_ptr<std::vector<ProtocolVertex>> pChilds);
        void    discover_vertex(ProtocolVertex vertexId, const ProtocolGraph& graph);

    private:

        bool                                            m_bFirst = true;
        std::shared_ptr<std::vector<ProtocolVertex>>    m_pChilds = nullptr;
};

//------------------------------//
//- Class CProgressStepVisitor -//
//------------------------------//
class CProgressStepVisitor: public boost::default_bfs_visitor
{
    public:

        CProgressStepVisitor(size_t unitEltCount, size_t& stepCount);
        CProgressStepVisitor(size_t&&) = delete; //prevent rvalue binding

        void    discover_vertex(ProtocolVertex vertexId, const ProtocolGraph& graph);

    private:

        size_t     m_unitEltCount;
        size_t&    m_stepcount;
};

//------------------------------//
//- Class CElapsedTimeVisitor -//
//------------------------------//
class CElapsedTimeVisitor: public boost::default_bfs_visitor
{
    public:

        CElapsedTimeVisitor(double& elapsedTime);

        void    discover_vertex(ProtocolVertex vertexId, const ProtocolGraph& graph);

    private:

        double& m_elapsedTime;
};

//------------------------------//
//- Class CNotifyVideoVisitor -//
//------------------------------//
class CNotifyVideoVisitor: public boost::default_bfs_visitor
{
    public:

        CNotifyVideoVisitor(CProtocol* pProtocol, int frameCount);

        void    discover_vertex(ProtocolVertex vertexId, const ProtocolGraph& graph);

    private:

        CProtocol*  m_pProtocol = nullptr;
        int         m_frameCount = 0;
};

//----------------------------//
//- Class CGenericDfsVisitor -//
//----------------------------//
class CGenericDfsVisitor : public boost::default_dfs_visitor
{
    public:

        CGenericDfsVisitor(std::function<void(const ProtocolVertex& vertexId)> applyToVertex);

        void    discover_vertex(ProtocolVertex vertexId, const ProtocolGraph& graph);

    private:

        using VertexFunc = std::function<void(const ProtocolVertex& vertexId)>;
        VertexFunc  m_applyFunc = nullptr;
};

using ProtocolUPtr = std::unique_ptr<CProtocol>;
using ProtocolPtr = std::shared_ptr<CProtocol>;

#endif // CPROTOCOL_H
