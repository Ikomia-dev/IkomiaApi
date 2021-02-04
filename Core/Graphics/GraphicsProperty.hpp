#ifndef GRAPHICSPROPERTY_HPP
#define GRAPHICSPROPERTY_HPP

#include "Main/CoreDefine.hpp"

/** @file GraphicsProperty.hpp */

struct GraphicsEllipseProperty
{
    CColor      m_penColor = {255,0,0,255};
    CColor      m_brushColor =  {255,0,0,0};
    int         m_lineSize = 2;
    std::string m_category = "Default";
};

struct GraphicsPointProperty
{
    CColor      m_penColor = {255,0,0,255};
    CColor      m_brushColor = {255,0,0,255};
    int         m_size = 3;
    std::string m_category = "Default";
};

struct GraphicsPolygonProperty
{
    CColor      m_penColor = {255,0,0,255};
    CColor      m_brushColor = {255,0,0,0};
    int         m_lineSize = 2;
    std::string m_category = "Default";
};

struct GraphicsPolylineProperty
{
    CColor      m_penColor = {255,0,0,255};
    int         m_lineSize = 2;
    std::string m_category = "Default";
};

struct GraphicsRectProperty
{
    CColor      m_penColor = {255,0,0,255};
    CColor      m_brushColor = {255,0,0,0};
    int         m_lineSize = 2;
    std::string m_category = "Default";
};

struct GraphicsTextProperty
{
    CColor      m_color = {255,0,0,255};
    std::string m_fontName = "Helvetica";
    int         m_fontSize = 12;
    bool        m_bBold = false;
    bool        m_bItalic = false;
    bool        m_bUnderline = false;
    bool        m_bStrikeOut = false;
};

#endif // GRAPHICSPROPERTY_HPP
