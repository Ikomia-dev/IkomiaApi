#ifndef CRUNTASKMANAGER_H
#define CRUNTASKMANAGER_H

#include "Protocol/CProtocolTask.h"

class CRunTaskManager
{
    public:

        CRunTaskManager();

        void    setBatchMode(bool bEnable);

        void    run(const ProtocolTaskPtr& pTask);

        void    stop(const ProtocolTaskPtr &taskPtr);

    private:

        void    runImageProcess2D(const ProtocolTaskPtr& taskPtr);
        void    runVideoProcess(const ProtocolTaskPtr& taskPtr);
        void    runWholeVideoProcess(const ProtocolTaskPtr& taskPtr);

    private:

        bool                m_bBatchMode = false;
        std::atomic_bool    m_bStop{false};
        size_t              m_outputVideoId = SIZE_MAX;
};

#endif // CRUNTASKMANAGER_H
