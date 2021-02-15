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

#ifndef CDATAMANAGER_HPP
#define CDATAMANAGER_HPP

#include "Data/CDataset.hpp"
#include "CProgressSignalHandler.h"

/**
 * @brief
 *
 */
template <typename T>
class CDataManager
{
    public:

        CDataManager(){}

        virtual ~CDataManager(){}

        void            setProgressSignalHandler(CProgressSignalHandler* pHandler)
        {
            m_pProgressSignalHandler = pHandler;
        }

        virtual void    loadData(CDataset<T>& dataset) = 0;
        virtual void    loadData(CDataset<T>& dataset, const SubsetBounds& subset) = 0;

        virtual void    clearData() = 0;

    protected:

        void            emitSetProgressSteps(size_t steps)
        {
            if(m_pProgressSignalHandler)
                m_pProgressSignalHandler->emitSetTotalSteps(static_cast<int>(steps));
        }
        void            emitSetProgressMsg(const std::string& msg)
        {
            if(m_pProgressSignalHandler)
                m_pProgressSignalHandler->emitSetMessage(msg);
        }
        void            emitDoProgress()
        {
            if(m_pProgressSignalHandler)
                m_pProgressSignalHandler->emitProgress();
        }

    protected:

        CProgressSignalHandler* m_pProgressSignalHandler = nullptr;
};

#endif // CDATAMANAGER_HPP
