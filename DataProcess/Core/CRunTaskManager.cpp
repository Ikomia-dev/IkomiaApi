#include "CRunTaskManager.h"
#include "Data/CDataVideoInfo.h"
#include "IO/CImageProcessIO.h"
#include "IO/CVideoProcessIO.h"
#include "Main/CoreTools.hpp"

CRunTaskManager::CRunTaskManager()
{
}

void CRunTaskManager::setBatchMode(bool bEnable)
{
    m_bBatchMode = bEnable;
}

void CRunTaskManager::run(const ProtocolTaskPtr &pTask)
{
    m_bStop = false;

    if(pTask == nullptr)
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Invalid task").toStdString(), __func__, __FILE__, __LINE__);

    switch(pTask->getType())
    {
        case CProtocolTask::Type::GENERIC:
        case CProtocolTask::Type::DNN_TRAIN:
            pTask->run();
            break;
        case CProtocolTask::Type::IMAGE_PROCESS_2D:
            runImageProcess2D(pTask);
            break;
        case CProtocolTask::Type::IMAGE_PROCESS_3D:
            pTask->run();
            break;
        case CProtocolTask::Type::VIDEO:
            runVideoProcess(pTask);
            break;
    }
}

void CRunTaskManager::stop(const ProtocolTaskPtr &taskPtr)
{
    m_bStop = true;
    if(taskPtr)
        taskPtr->stop();
}

void CRunTaskManager::runImageProcess2D(const ProtocolTaskPtr &taskPtr)
{
    //Thread safety -> scoped lock for all inputs/outputs
    //Access through CObjectLocker<CProtocolTaskIO> ioLock(*ioPtr);
    //auto inputLocks = taskPtr->createInputScopedLocks();
    //auto outputLocks = taskPtr->createOutputScopedLocks();
    const std::set<IODataType> volumeTypes = {IODataType::VOLUME, IODataType::VOLUME_LABEL, IODataType::VOLUME_BINARY};
    const std::set<IODataType> videoTypes = {IODataType::VIDEO, IODataType::VIDEO_LABEL, IODataType::VIDEO_BINARY};

    if(taskPtr->hasInput(volumeTypes) &&
        ((taskPtr->isActionFlagEnable(CProtocolTask::ActionFlag::APPLY_VOLUME)) || m_bBatchMode == true))
    {
        //Get volume inputs
        auto imageInputs = taskPtr->getInputs(volumeTypes);
        if(imageInputs.size() == 0)
            return;

        //Check volume size
        CMat firstVolume = std::static_pointer_cast<CImageProcessIO>(imageInputs[0])->getData();
        for(size_t i=1; i<imageInputs.size(); ++i)
        {
            if(imageInputs[i]->isDataAvailable())
            {
                CMat volume = std::static_pointer_cast<CImageProcessIO>(imageInputs[i])->getData();
                if(firstVolume.size != volume.size)
                    throw CException(CoreExCode::INVALID_SIZE, QObject::tr("Different volume dimensions").toStdString(), __func__, __FILE__, __LINE__);
            }
        }

        //Run process on each 2D images
        std::vector<CMat> volumes;
        for(size_t i=0; i<firstVolume.getNbStacks() && m_bStop==false; ++i)
        {
            //Set the current image from the 3D input to process
            for(size_t j=0; j<imageInputs.size(); ++j)
                std::static_pointer_cast<CImageProcessIO>(imageInputs[j])->setCurrentImage(i);

            //Run process
            taskPtr->run();

            if(i == 0)
            {
                //Get volume outputs and allocate 3D CMat
                auto volumeOutputs = taskPtr->getOutputs(volumeTypes);
                for(size_t j=0; j<volumeOutputs.size(); ++j)
                {
                    auto ouputPtr = std::static_pointer_cast<CImageProcessIO>(volumeOutputs[j]);
                    volumes.emplace_back(CMat((int)firstVolume.getNbRows(), (int)firstVolume.getNbCols(), (int)firstVolume.getNbStacks(), ouputPtr->getImage().type()));
                }
            }

            //Insert 2D image into output 3D CMat
            for(size_t j=0; j<volumes.size(); ++j)
            {
                auto pOutput = std::static_pointer_cast<CImageProcessIO>(taskPtr->getOutput(j));
                volumes[j].setPlane(i, pOutput->getImage());
            }
        }

        //Set final output of task
        for(size_t i=0; i<volumes.size(); ++i)
        {
            auto pOutput = std::static_pointer_cast<CImageProcessIO>(taskPtr->getOutput(i));
            pOutput->setImage(volumes[i]);
        }
    }
    else if(taskPtr->hasInput(videoTypes) && m_bBatchMode == true)
    {
        runWholeVideoProcess(taskPtr);
    }
    else
    {
        taskPtr->run();
    }
}

void CRunTaskManager::runVideoProcess(const ProtocolTaskPtr& taskPtr)
{
    if(m_bBatchMode == true)
    {
        runWholeVideoProcess(taskPtr);
    }
    else
    {
        const std::set<IODataType> videoTypes = {
            IODataType::VIDEO, IODataType::VIDEO_LABEL, IODataType::VIDEO_BINARY,
            IODataType::LIVE_STREAM, IODataType::LIVE_STREAM_LABEL, IODataType::LIVE_STREAM_BINARY};

        //Get video inputs
        auto videoInputs = taskPtr->getInputs(videoTypes);
        if(videoInputs.size() == 0)
            return;

        //Run process
        taskPtr->run();
    }
}

void CRunTaskManager::runWholeVideoProcess(const ProtocolTaskPtr &taskPtr)
{
    const std::set<IODataType> videoTypes = {IODataType::VIDEO, IODataType::VIDEO_LABEL, IODataType::VIDEO_BINARY};

    //Get video inputs
    auto videoInputs = taskPtr->getInputs(videoTypes);
    if(videoInputs.size() == 0)
        return;

    //Get video outputs
    auto videoOutputs = taskPtr->getOutputs(videoTypes);
    if(videoOutputs.size() == 0)
        return;

    for(size_t i=0; i<videoInputs.size(); ++i)
    {
        auto inputPtr = std::static_pointer_cast<CVideoProcessIO>(videoInputs[i]);
        // Set video position to the first image for processing all the video
        inputPtr->setVideoPos(0);
        // Start acquisition
        inputPtr->startVideo();
    }

    auto infoPtr = std::static_pointer_cast<CDataVideoInfo>(videoInputs[0]->getDataInfo());
    for(size_t i=0; i<videoOutputs.size(); ++i)
    {
        auto outputPtr = std::static_pointer_cast<CVideoProcessIO>(videoOutputs[i]);
        // Start video write
        Utils::File::createDirectory(taskPtr->getOutputFolder());
        std::string outPath = taskPtr->getOutputFolder() + taskPtr->getName() + "_" + std::to_string(i+1) + ".avi";
        outputPtr->addTemporaryFile(outPath);
        outputPtr->setVideoPath(outPath);
        outputPtr->startVideoWrite(infoPtr->m_width, infoPtr->m_height, infoPtr->m_frameCount, infoPtr->m_fps, infoPtr->m_fourcc);
    }

    for(int i=0; i<infoPtr->m_frameCount && !m_bStop; ++i)
    {
        try
        {
            //Run process
            taskPtr->run();

            for(size_t j=0; j<videoOutputs.size(); ++j)
            {
                auto outputPtr = std::static_pointer_cast<CVideoProcessIO>(videoOutputs[j]);
                CMat img = outputPtr->getImage();
                outputPtr->writeImage(img);
            }
        }
        catch(CException& e)
        {
            if(e.getCode() == CoreExCode::TIMEOUT_REACHED)
                break;
        }
    }

    for(size_t i=0; i<videoInputs.size(); ++i)
    {
        auto inputPtr = std::static_pointer_cast<CVideoProcessIO>(videoInputs[i]);
        inputPtr->stopVideo();
    }

    for(size_t i=0; i<videoOutputs.size(); ++i)
    {
        auto outputPtr = std::static_pointer_cast<CVideoProcessIO>(videoOutputs[i]);
        outputPtr->waitWriteFinished();
        outputPtr->stopVideoWrite();
        outputPtr->setVideoPos(0);
    }
}

