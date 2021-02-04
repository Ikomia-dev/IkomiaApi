#ifndef CVIDEOPROCESSWRAP_H
#define CVIDEOPROCESSWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CVideoProcess.h"

//----------------------------------------------------------//
//-----------      CVideoProcessWrap           -------------//
//- Wrapping class to handle virtual functions and signals -//
//----------------------------------------------------------//
class CVideoProcessWrap: public CVideoProcess, public wrapper<CVideoProcess>
{
    public:

        CVideoProcessWrap();
        CVideoProcessWrap(const std::string& name);
        CVideoProcessWrap(const CVideoProcess& process);

        virtual size_t  getProgressSteps();
        size_t          default_getProgressSteps();

        virtual size_t  getProgressSteps(size_t unitEltCount);
        size_t          default_getProgressSteps(size_t unitEltCount);

        virtual void    setActive(bool bActive);
        void            default_setActive(bool bActive);

        virtual void    updateStaticOutputs();
        void            default_updateStaticOutputs();

        virtual void    beginTaskRun();
        void            default_beginTaskRun();

        virtual void    endTaskRun();
        void            default_endTaskRun();

        virtual void    run();
        void            default_run();

        virtual void    stop();
        void            default_stop();

        virtual void    graphicsChanged();
        void            default_graphicsChanged();

        virtual void    globalInputChanged(bool bNewSequence);
        void            default_globalInputChanged(bool bNewSequence);

        virtual void    notifyVideoStart(int frameCount);
        void            default_notifyVideoStart(int frameCount);

        virtual void    notifyVideoEnd();
        void            default_notifyVideoEnd();

        void            emitStepProgress();
        void            emitGraphicsContextChanged();
        void            emitOutputChanged();
};

#endif // CVIDEOPROCESSWRAP_H
