#include "CInteractiveImageProcess2d.h"

CInteractiveImageProcess2d::CInteractiveImageProcess2d() : CImageProcess2d()
{
}

CInteractiveImageProcess2d::CInteractiveImageProcess2d(const std::string &name) : CImageProcess2d(name)
{
}

CInteractiveImageProcess2d::CInteractiveImageProcess2d(const CInteractiveImageProcess2d &process) : CImageProcess2d(process)
{
    auto pLayer = process.m_layerPtr.get();
    m_layerPtr = std::make_unique<CGraphicsLayer>(*pLayer);
    m_interactionMask = process.m_interactionMask;
    m_hierarchy = process.m_hierarchy;
    m_blobs = process.m_blobs;
}

void CInteractiveImageProcess2d::setActive(bool bActive)
{
    CImageProcess2d::setActive(bActive);

    if(bActive == true)
    {
        if(m_layerPtr == nullptr)
            m_layerPtr = std::make_unique<CGraphicsLayer>(QString::fromStdString(getName()), nullptr);

        emit m_signalHandler->doAddGraphicsLayer(m_layerPtr.get());
    }
    else
        emit m_signalHandler->doRemoveGraphicsLayer(m_layerPtr.get());
}

CMat CInteractiveImageProcess2d::getInteractionMask() const
{
    return m_interactionMask;
}

CInteractiveImageProcess2d::Blobs CInteractiveImageProcess2d::getBlobs() const
{
    return m_blobs;
}

QGraphicsItem *CInteractiveImageProcess2d::getLastItem() const
{
    QGraphicsItem* pLast = nullptr;
    if(m_layerPtr)
    {
        auto items = m_layerPtr->getChildItems();
        if(items.size() > 0)
            pLast = items.back();
    }
    return pLast;
}

CGraphicsLayer *CInteractiveImageProcess2d::getInteractionLayer() const
{
    return m_layerPtr.get();
}

bool CInteractiveImageProcess2d::isGraphicsChangedListening() const
{
    return true;
}

void CInteractiveImageProcess2d::graphicsChanged()
{
    CImageProcess2d::graphicsChanged();
}

void CInteractiveImageProcess2d::globalInputChanged(bool bNewSequence)
{
    CImageProcess2d::globalInputChanged(bNewSequence);

    //If source image is set, we have to clear interaction layer
    if(m_layerPtr && bNewSequence == true)
    {
        m_layerPtr->deleteChildItems();
        emit m_signalHandler->doAddGraphicsLayer(m_layerPtr.get());
    }
}

void CInteractiveImageProcess2d::createInteractionMask(int width, int height)
{
    m_interactionMask.release();

    if(m_layerPtr == nullptr)
        return;

    //Generate graphics masks
    CGraphicsConversion graphicsConv(width, height);
    auto graphicsItems = m_layerPtr->getChildItems();
    m_interactionMask = graphicsConv.graphicsToBinaryMask(graphicsItems);
}

void CInteractiveImageProcess2d::computeBlobs()
{
    cv::findContours(m_interactionMask, m_blobs, m_hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_TC89_L1);
}

void CInteractiveImageProcess2d::clearInteractionLayer()
{
    m_layerPtr->deleteChildItems();
}
