#include "CWorkflowWrap.h"

CWorkflowWrap::CWorkflowWrap() : CWorkflow()
{
}

CWorkflowWrap::CWorkflowWrap(const std::string &name) : CWorkflow(name)
{
}

CWorkflowWrap::CWorkflowWrap(const std::string& name, const std::shared_ptr<CIkomiaRegistry> &registryPtr)
    : CWorkflow(name, registryPtr->getTaskRegistrator(), registryPtr->getIORegistrator(), nullptr)
{
}

CWorkflowWrap::CWorkflowWrap(const CWorkflow &workflow): CWorkflow(workflow)
{
}

intptr_t CWorkflowWrap::getRootID()
{
    return reinterpret_cast<std::intptr_t>(getRootId());
}

std::vector<std::intptr_t> CWorkflowWrap::getTaskIDs()
{
    std::vector<std::intptr_t> nodes;
    auto vertices = getVertices();

    for(auto it=vertices.first; it!=vertices.second; ++it)
        nodes.push_back(reinterpret_cast<std::intptr_t>(*it));

    return nodes;
}

WorkflowTaskPtr CWorkflowWrap::getTask(intptr_t id)
{
    WorkflowVertex vertex = reinterpret_cast<WorkflowVertex>(id);
    return CWorkflow::getTask(vertex);
}

double CWorkflowWrap::getElapsedTimeTo(intptr_t id)
{
    WorkflowVertex vertex = reinterpret_cast<WorkflowVertex>(id);
    return CWorkflow::getElapsedTimeTo(vertex);
}

std::vector<intptr_t> CWorkflowWrap::getParents(intptr_t id)
{
    WorkflowVertex vertex = reinterpret_cast<WorkflowVertex>(id);
    auto vertices = CWorkflow::getParents(vertex);

    std::vector<intptr_t> parents;
    for(size_t i=0; i<vertices.size(); ++i)
        parents.push_back(reinterpret_cast<std::intptr_t>(vertices[i]));

    return parents;
}

std::vector<intptr_t> CWorkflowWrap::getChilds(intptr_t id)
{
    WorkflowVertex vertex = reinterpret_cast<WorkflowVertex>(id);
    auto vertices = CWorkflow::getChilds(vertex);

    std::vector<intptr_t> childs;
    for(size_t i=0; i<vertices.size(); ++i)
        childs.push_back(reinterpret_cast<std::intptr_t>(vertices[i]));

    return childs;
}

std::vector<size_t> CWorkflowWrap::getInEdges(intptr_t id)
{
    WorkflowVertex vertex = reinterpret_cast<WorkflowVertex>(id);
    auto edgeRange = CWorkflow::getInEdges(vertex);

    std::vector<size_t> edges;
    for(auto it=edgeRange.first; it!=edgeRange.second; ++it)
    {
        auto itIndex = m_edgeDescToIndex.find(*it);
        if(itIndex != m_edgeDescToIndex.end())
            edges.push_back(itIndex->second);
    }
    return edges;
}

std::vector<size_t> CWorkflowWrap::getOutEdges(intptr_t id)
{
    WorkflowVertex vertex = reinterpret_cast<WorkflowVertex>(id);
    auto edgeRange = CWorkflow::getOutEdges(vertex);

    std::vector<size_t> edges;
    for(auto it=edgeRange.first; it!=edgeRange.second; ++it)
    {
        auto itIndex = m_edgeDescToIndex.find(*it);
        if(itIndex != m_edgeDescToIndex.end())
            edges.push_back(itIndex->second);
    }
    return edges;
}

tuple CWorkflowWrap::getEdgeInfo(size_t id)
{
    auto retPair = getEdgeDescriptor(id);
    if(retPair.first)
    {
        auto edgePtr = CWorkflow::getEdge(retPair.second);
        return make_tuple(edgePtr->getSourceIndex(), edgePtr->getTargetIndex());
    }
    else
        return make_tuple(-1, -1);
}

intptr_t CWorkflowWrap::getEdgeSource(size_t id)
{
    auto retPair = getEdgeDescriptor(id);
    if(retPair.first)
    {
        auto v = CWorkflow::getEdgeSource(retPair.second);
        return reinterpret_cast<intptr_t>(v);
    }
    else
        return 0;
}

intptr_t CWorkflowWrap::getEdgeTarget(size_t id)
{
    auto retPair = getEdgeDescriptor(id);
    if(retPair.first)
    {
        auto v = CWorkflow::getEdgeTarget(retPair.second);
        return reinterpret_cast<intptr_t>(v);
    }
    else
        return 0;
}

intptr_t CWorkflowWrap::addTaskWrap(const WorkflowTaskPtr &taskPtr)
{
    auto vertex = addTask(taskPtr);
    return reinterpret_cast<std::intptr_t>(vertex);
}

void CWorkflowWrap::connectWrap(const std::intptr_t &src, const std::intptr_t &target, int srcIndex, int targetIndex)
{
    auto srcVertex = reinterpret_cast<WorkflowVertex>(src);
    auto targetVertex = reinterpret_cast<WorkflowVertex>(target);

    if(srcIndex == -1 && targetIndex == -1)
    {
        // Auto connection
        auto edges = connect(srcVertex, targetVertex);
        for(size_t i=0; i<edges.size(); ++i)
            m_edgeDescToIndex.insert(std::make_pair(edges[i], m_edgeIndex++));
    }
    else
    {
        auto edge = connect(srcVertex, srcIndex, targetVertex, targetIndex);
        m_edgeDescToIndex.insert(std::make_pair(edge, m_edgeIndex++));
    }
}

void CWorkflowWrap::deleteTaskWrap(intptr_t id)
{
    auto v = reinterpret_cast<WorkflowVertex>(id);

    auto inEdgeIds = CWorkflow::getInEdges(v);
    for(auto it=inEdgeIds.first; it!=inEdgeIds.second; ++it)
        removeEdgeIndex(*it);

    auto outEdgeIds = CWorkflow::getOutEdges(v);
    for(auto it=outEdgeIds.first; it!=outEdgeIds.second; ++it)
        removeEdgeIndex(*it);

    CWorkflow::deleteTask(v);
}

void CWorkflowWrap::deleteEdgeWrap(size_t id)
{
    auto retPair = getEdgeDescriptor(id);
    if(retPair.first)
    {
        CWorkflow::deleteEdge(retPair.second);
        removeEdgeIndex(retPair.second);
    }
}

void CWorkflowWrap::run()
{
    CPyEnsureGIL gil;
    try
    {
        if(override runOver = this->get_override("run"))
            runOver();
        else
            CWorkflow::run();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWorkflowWrap::default_run()
{
    CPyEnsureGIL gil;
    try
    {
        this->CWorkflow::run();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWorkflowWrap::clearWrap()
{
    CWorkflow::clear();
    m_edgeDescToIndex.clear();
}

void CWorkflowWrap::loadWrap(const std::string &path)
{
    load(path);
    auto edges = getEdges();

    m_edgeDescToIndex.clear();
    for(auto it=edges.first; it!=edges.second; ++it)
        m_edgeDescToIndex.insert(std::make_pair(*it, m_edgeIndex++));
}

std::pair<bool, WorkflowEdge> CWorkflowWrap::getEdgeDescriptor(size_t index) const
{
    for(auto it : m_edgeDescToIndex)
    {
        if(it.second == index)
            return std::make_pair(true, it.first);
    }
    return std::make_pair(false, WorkflowEdge());
}

void CWorkflowWrap::removeEdgeIndex(const WorkflowEdge& edge)
{
    auto it = m_edgeDescToIndex.find(edge);
    if(it != m_edgeDescToIndex.end())
        m_edgeDescToIndex.erase(it);
}
