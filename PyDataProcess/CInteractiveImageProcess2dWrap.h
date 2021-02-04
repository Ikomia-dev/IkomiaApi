#ifndef CINTERACTIVEIMAGEPROCESS2DWRAP_H
#define CINTERACTIVEIMAGEPROCESS2DWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CInteractiveImageProcess2d.h"

//----------------------------------------------------------//
//-----------  CInteractiveImageProcess2dWrap  -------------//
//- Wrapping class to handle virtual functions and signals -//
//----------------------------------------------------------//
class CInteractiveImageProcess2dWrap: public CInteractiveImageProcess2d, public wrapper<CInteractiveImageProcess2d>
{
    public:

        CInteractiveImageProcess2dWrap();
        CInteractiveImageProcess2dWrap(const std::string& name);
        CInteractiveImageProcess2dWrap(const CInteractiveImageProcess2d& process);

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

#endif // CINTERACTIVEIMAGEPROCESS2DWRAP_H
