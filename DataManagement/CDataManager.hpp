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
