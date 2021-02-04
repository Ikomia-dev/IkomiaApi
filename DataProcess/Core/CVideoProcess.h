#ifndef CVIDEOPROCESS_H
#define CVIDEOPROCESS_H

#include "CImageProcess2d.h"
#include "IO/CVideoProcessIO.h"
#include "DataProcessGlobal.hpp"

/**
 * @ingroup groupDataProcess
 * @brief The CVideoProcess class is derived from CImageProcess2d and adds video specific features.
 * It should be the base class for all process task dedicated to video processing.
 */
class DATAPROCESSSHARED_EXPORT CVideoProcess : public CImageProcess2d
{
    public:

        /**
         * @brief Default constructor.
         * Same inputs and outputs as CImageProcess2d.
         */
        CVideoProcess();
        /**
         * @brief Constructs CVideoProcess with the given name.
         * Same inputs and outputs as CImageProcess2d.
         * @param name: name of the task.
         */
        CVideoProcess(const std::string& name);

        virtual ~CVideoProcess() = default;

        /**
         * @brief Performs checks before the process is started
         */
        virtual void    beginTaskRun() override;
        /**
         * @brief Notifies that the video has started.
         * @param frameCount: total number of frames.
         */
        virtual void    notifyVideoStart(int frameCount) override;
        /**
         * @brief Notifies that the end of the video is reached.
         */
        virtual void    notifyVideoEnd() override;
};

#endif // CVIDEOPROCESS_H
