#ifndef CPROTOCOLEDGE_HPP
#define CPROTOCOLEDGE_HPP

class CProtocolEdge
{
    public:

        CProtocolEdge(){}
        CProtocolEdge(size_t source, size_t target)
        {
            m_sourceIndex = source;
            m_targetIndex = target;
        }
        virtual ~CProtocolEdge(){}

        size_t  getSourceIndex() const
        {
            return m_sourceIndex;
        }
        size_t  getTargetIndex() const
        {
            return m_targetIndex;
        }
        uint    getHashValue() const
        {
            return qHash(std::make_pair(m_sourceIndex, m_targetIndex));
        }

        void    setSourceIndex(size_t index)
        {
            m_sourceIndex = index;
        }
        void    setTargetIndex(size_t index)
        {
            m_targetIndex = index;
        }

    private:

        size_t     m_sourceIndex = 0;
        size_t     m_targetIndex = 0;
};

using ProtocolEdgePtr = std::shared_ptr<CProtocolEdge>;

#endif // CPROTOCOLEDGE_HPP
