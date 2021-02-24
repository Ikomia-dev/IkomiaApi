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

#include "CViewPropertyIO.h"

CViewPropertyIO::CViewPropertyIO()
{    
}

void CViewPropertyIO::setMaximized(bool bMaximized)
{
    m_bChanged = m_bMaximized != bMaximized;
    m_bMaximized = bMaximized;
}

void CViewPropertyIO::setZoomFit(bool bFit)
{
    m_bChanged = m_bZoomFit != bFit;
    m_bZoomFit = bFit;
}

void CViewPropertyIO::setZoomScale(float scale)
{
    m_bChanged = m_zoomScale != scale;
    m_zoomScale = scale;
    m_bZoomFit = false;
}

void CViewPropertyIO::setZoomPosition(int numScheduledScalings, const QPointF &targetPos, const QPointF &viewCenter)
{
    m_zoomNumScheduledScalings = numScheduledScalings;
    m_zoomTargetPos = targetPos;
    m_viewCenter = viewCenter;
}

void CViewPropertyIO::setViewMode(const CViewPropertyIO::ViewMode mode)
{
    m_viewMode = mode;
}

float CViewPropertyIO::getZoomScale() const
{
    return m_zoomScale;
}

int CViewPropertyIO::getZoomNumScheduledScalings() const
{
    return m_zoomNumScheduledScalings;
}

QPointF CViewPropertyIO::getZoomTargetPos() const
{
    return m_zoomTargetPos;
}

QPointF CViewPropertyIO::getViewCenter() const
{
    return m_viewCenter;
}

CViewPropertyIO::ViewMode CViewPropertyIO::getViewMode() const
{
    return m_viewMode;
}

bool CViewPropertyIO::isChanged() const
{
    return m_bChanged;
}

bool CViewPropertyIO::isMaximized() const
{
    return m_bMaximized;
}

bool CViewPropertyIO::isZoomFit() const
{
    return m_bZoomFit;
}

void CViewPropertyIO::toggleMaximized()
{
    m_bMaximized = !m_bMaximized;
}
