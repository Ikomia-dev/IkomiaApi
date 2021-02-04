#include "CGraphicsContext.h"

CGraphicsContext::CGraphicsContext()
{
}

void CGraphicsContext::setTool(GraphicsShape tool)
{
    m_tool = tool;
}

void CGraphicsContext::setCurrentCategory(const std::string &category)
{
    m_pointProperty.m_category = category;
    m_ellipseProperty.m_category = category;
    m_rectProperty.m_category = category;
    m_polygonProperty.m_category = category;
    m_polylineProperty.m_category = category;
}

GraphicsShape CGraphicsContext::getTool() const
{
    return m_tool;
}

GraphicsPointProperty* CGraphicsContext::getPointPropertyPtr()
{
    return &m_pointProperty;
}

GraphicsPointProperty CGraphicsContext::getPointProperty() const
{
    return m_pointProperty;
}

GraphicsEllipseProperty *CGraphicsContext::getEllipsePropertyPtr()
{
    return &m_ellipseProperty;
}

GraphicsEllipseProperty CGraphicsContext::getEllipseProperty() const
{
    return m_ellipseProperty;
}

GraphicsRectProperty *CGraphicsContext::getRectPropertyPtr()
{
    return &m_rectProperty;
}

GraphicsRectProperty CGraphicsContext::getRectProperty() const
{
    return m_rectProperty;
}

GraphicsPolygonProperty *CGraphicsContext::getPolygonPropertyPtr()
{
    return &m_polygonProperty;
}

GraphicsPolygonProperty CGraphicsContext::getPolygonProperty() const
{
    return m_polygonProperty;
}

GraphicsPolylineProperty *CGraphicsContext::getPolylinePropertyPtr()
{
    return &m_polylineProperty;
}

GraphicsPolylineProperty CGraphicsContext::getPolylineProperty() const
{
    return m_polylineProperty;
}

GraphicsTextProperty *CGraphicsContext::getTextPropertyPtr()
{
    return &m_textProperty;
}

GraphicsTextProperty CGraphicsContext::getTextProperty() const
{
    return m_textProperty;
}
