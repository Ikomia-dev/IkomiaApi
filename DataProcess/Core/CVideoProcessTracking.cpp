#include "CVideoProcessTracking.h"
#include "Graphics/CGraphicsLayer.h"
#include "Graphics/CGraphicsRectangle.h"

CVideoProcessTracking::CVideoProcessTracking() : CVideoProcess()
{
    setOutputDataType(IODataType::IMAGE_BINARY, 0);
    addOutput(std::make_shared<CImageProcessIO>());
    addOutput(std::make_shared<CGraphicsProcessOutput>());
    addOutput(std::make_shared<CMeasureProcessIO>());
}

CVideoProcessTracking::CVideoProcessTracking(const std::string& name) : CVideoProcess(name)
{
    setOutputDataType(IODataType::IMAGE_BINARY, 0);
    addOutput(std::make_shared<CImageProcessIO>());
    addOutput(std::make_shared<CGraphicsProcessOutput>());
    addOutput(std::make_shared<CMeasureProcessIO>());
}

void CVideoProcessTracking::setRoiToTrack()
{
    auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
    if(pGraphicsInput == nullptr)
        return;

    auto items = pGraphicsInput->getItems();
    if(items.size() == 0)
    {
        qCritical().noquote() << QObject::tr("Tracker requires rectangular object");
        return;
    }

    if(items.size() > 1)
        qWarning().noquote() << QObject::tr("Tracker can only track one rectangular object");

    auto pItem = std::dynamic_pointer_cast<CProxyGraphicsRect>(items.back());
    if(pItem == nullptr)
    {
        qCritical().noquote() << QObject::tr("Tracker can only track rectangular object");
        return;
    }

    m_trackedRect = cv::Rect2d(pItem->m_x, pItem->m_y, pItem->m_width, pItem->m_height);
    m_bInitRoi = true;
}

void CVideoProcessTracking::manageOutputs()
{
    forwardInputImage(0, 1);

    //Generate binary mask
    auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
    auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));

    if(pOutput)
    {
        CMat imgSrc = pInput->getImage();
        CMat imgDst(imgSrc.rows, imgSrc.cols, CV_8UC1, cv::Scalar(0));
        CMat roi(imgDst, m_trackedRect);
        roi.setTo(cv::Scalar(255));
        pOutput->setImage(imgDst);
    }

    //Graphics output
    auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(2));
    pGraphicsOutput->setNewLayer("Trackerlayer");
    pGraphicsOutput->setImageIndex(1);

    //Create rectangle graphics of bbox
    auto graphicsObj = pGraphicsOutput->addRectangle(m_trackedRect.x, m_trackedRect.y, m_trackedRect.width, m_trackedRect.height);

    //Tracked rectangle coordinates
    auto pMeasureOutput = std::dynamic_pointer_cast<CMeasureProcessIO>(getOutput(3));
    if(pMeasureOutput)
    {
        CMeasure bboxMeasure(CMeasure::BBOX, QObject::tr("Tracked ROI").toStdString());
        pMeasureOutput->setObjectMeasure(0, CObjectMeasure(bboxMeasure, {(double)m_trackedRect.x, (double)m_trackedRect.y, (double)m_trackedRect.width, (double)m_trackedRect.height}, graphicsObj->getId(), ""));
    }
}

void CVideoProcessTracking::notifyVideoStart(int frameCount)
{
    Q_UNUSED(frameCount);
    m_bVideoStarted = true;
    m_bInitRoi = false;
}

void CVideoProcessTracking::notifyVideoEnd()
{
    m_bVideoStarted = false;
}
