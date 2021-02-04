#ifndef CVIDEOPROCESSOFWRAP_H
#define CVIDEOPROCESSOFWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CVideoProcessOF.h"

//----------------------------------------------------------//
//-----------      CVideoProcessOFWrap         -------------//
//- Wrapping class to handle virtual functions and signals -//
//----------------------------------------------------------//
class CVideoProcessOFWrap: public CVideoProcessOF, public wrapper<CVideoProcessOF>
{
    public:

        CVideoProcessOFWrap();
        CVideoProcessOFWrap(const std::string& name);
        CVideoProcessOFWrap(const CVideoProcessOF &process);

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

#endif // CVIDEOPROCESSOFWRAP_H
