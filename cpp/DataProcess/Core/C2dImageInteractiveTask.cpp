// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "C2dImageInteractiveTask.h"

C2dImageInteractiveTask::C2dImageInteractiveTask() : C2dImageTask()
{
}

C2dImageInteractiveTask::C2dImageInteractiveTask(const std::string &name) : C2dImageTask(name)
{
}

C2dImageInteractiveTask::C2dImageInteractiveTask(const C2dImageInteractiveTask &process) : C2dImageTask(process)
{
    auto pLayer = process.m_layerPtr.get();
    m_layerPtr = std::make_unique<CGraphicsLayer>(*pLayer);
    m_interactionMask = process.m_interactionMask;
    m_hierarchy = process.m_hierarchy;
    m_blobs = process.m_blobs;
}

void C2dImageInteractiveTask::setActive(bool bActive)
{
    C2dImageTask::setActive(bActive);

    if(bActive == true)
    {
        if(m_layerPtr == nullptr)
            m_layerPtr = std::make_unique<CGraphicsLayer>(QString::fromStdString(getName()), nullptr);

        emit m_signalHandler->doAddGraphicsLayer(m_layerPtr.get());
    }
    else
        emit m_signalHandler->doRemoveGraphicsLayer(m_layerPtr.get());
}

CMat C2dImageInteractiveTask::getInteractionMask() const
{
    return m_interactionMask;
}

C2dImageInteractiveTask::Blobs C2dImageInteractiveTask::getBlobs() const
{
    return m_blobs;
}

QGraphicsItem *C2dImageInteractiveTask::getLastItem() const
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

CGraphicsLayer *C2dImageInteractiveTask::getInteractionLayer() const
{
    return m_layerPtr.get();
}

bool C2dImageInteractiveTask::isGraphicsChangedListening() const
{
    return true;
}

void C2dImageInteractiveTask::graphicsChanged()
{
    C2dImageTask::graphicsChanged();
}

void C2dImageInteractiveTask::globalInputChanged(bool bNewSequence)
{
    C2dImageTask::globalInputChanged(bNewSequence);

    //If source image is set, we have to clear interaction layer
    if(m_layerPtr && bNewSequence == true)
    {
        m_layerPtr->deleteChildItems();
        emit m_signalHandler->doAddGraphicsLayer(m_layerPtr.get());
    }
}

void C2dImageInteractiveTask::createInteractionMask(int width, int height)
{
    m_interactionMask.release();

    if(m_layerPtr == nullptr)
        return;

    //Generate graphics masks
    CGraphicsConversion graphicsConv(width, height);
    auto graphicsItems = m_layerPtr->getChildItems();
    m_interactionMask = graphicsConv.graphicsToBinaryMask(graphicsItems);
}

void C2dImageInteractiveTask::computeBlobs()
{
    cv::findContours(m_interactionMask, m_blobs, m_hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_TC89_L1);
}

void C2dImageInteractiveTask::clearInteractionLayer()
{
    m_layerPtr->deleteChildItems();
}
