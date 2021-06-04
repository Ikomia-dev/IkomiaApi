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

#ifndef CWORKFLOWEDGE_HPP
#define CWORKFLOWEDGE_HPP

class CWorkflowEdge
{
    public:

        CWorkflowEdge(){}
        CWorkflowEdge(size_t source, size_t target)
        {
            m_sourceIndex = source;
            m_targetIndex = target;
        }
        virtual ~CWorkflowEdge(){}

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

using WorkflowEdgePtr = std::shared_ptr<CWorkflowEdge>;

#endif // CWORKFLOWEDGE_HPP
