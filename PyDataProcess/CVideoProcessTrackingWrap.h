#ifndef CVIDEOPROCESSTRACKINGWRAP_H
#define CVIDEOPROCESSTRACKINGWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CVideoProcessTracking.h"

//----------------------------------------------------------//
//-----------    CVideoProcessTrackingWrap     -------------//
//- Wrapping class to handle virtual functions and signals -//
//----------------------------------------------------------//
class CVideoProcessTrackingWrap : public CVideoProcessTracking, public wrapper<CVideoProcessTracking>
{
    public:

        CVideoProcessTrackingWrap();
        CVideoProcessTrackingWrap(const std::string& name);
        CVideoProcessTrackingWrap(const CVideoProcessTracking &process);

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

        void            emitStepProgress();
        void            emitGraphicsContextChanged();
        void            emitOutputChanged();
};

#endif // CVIDEOPROCESSTRACKINGWRAP_H
