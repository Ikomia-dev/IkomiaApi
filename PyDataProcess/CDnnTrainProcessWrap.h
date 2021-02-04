#ifndef CDNNTRAINPROCESSWRAP_H
#define CDNNTRAINPROCESSWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CDnnTrainProcess.h"


class CDnnTrainProcessWrap : public CDnnTrainProcess, public wrapper<CDnnTrainProcess>
{
    public:

        CDnnTrainProcessWrap();
        CDnnTrainProcessWrap(const std::string& name, const std::shared_ptr<CDnnTrainProcessParam>& pParam);
        CDnnTrainProcessWrap(const CDnnTrainProcess& process);

        virtual size_t  getProgressSteps() override;
        size_t          default_getProgressSteps();

        virtual size_t  getProgressSteps(size_t unitEltCount) override;
        size_t          default_getProgressSteps(size_t unitEltCount);

        virtual void    setActive(bool bActive) override;
        void            default_setActive(bool bActive);

        virtual void    beginTaskRun() override;
        void            default_beginTaskRun();

        virtual void    endTaskRun() override;
        void            default_endTaskRun();

        virtual void    run() override;
        void            default_run();

        virtual void    stop() override;
        void            default_stop();

        void            emitStepProgress();
        void            emitOutputChanged();
};

#endif // CDNNTRAINPROCESSWRAP_H
