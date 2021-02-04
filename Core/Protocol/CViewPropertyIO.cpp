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
