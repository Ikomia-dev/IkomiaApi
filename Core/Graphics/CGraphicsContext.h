#ifndef CGLOBALGRAPHICSCONTEXT_H
#define CGLOBALGRAPHICSCONTEXT_H

#include <memory>
#include "Main/CoreGlobal.hpp"
#include "Graphics/GraphicsProperty.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CGraphicsContext
{
    public:

        CGraphicsContext();

        void                        setTool(GraphicsShape tool);
        void                        setCurrentCategory(const std::string &category);

        GraphicsShape               getTool() const;
        GraphicsPointProperty*      getPointPropertyPtr();
        GraphicsPointProperty       getPointProperty() const;
        GraphicsEllipseProperty*    getEllipsePropertyPtr();
        GraphicsEllipseProperty     getEllipseProperty() const;
        GraphicsRectProperty*       getRectPropertyPtr();
        GraphicsRectProperty        getRectProperty() const;
        GraphicsPolygonProperty*    getPolygonPropertyPtr();
        GraphicsPolygonProperty     getPolygonProperty() const;
        GraphicsPolylineProperty*   getPolylinePropertyPtr();
        GraphicsPolylineProperty    getPolylineProperty() const;
        GraphicsTextProperty*       getTextPropertyPtr();
        GraphicsTextProperty        getTextProperty() const;

    private:

        GraphicsShape               m_tool = GraphicsShape::SELECTION;
        GraphicsPointProperty       m_pointProperty;
        GraphicsEllipseProperty     m_ellipseProperty;
        GraphicsRectProperty        m_rectProperty;
        GraphicsPolygonProperty     m_polygonProperty;
        GraphicsPolylineProperty    m_polylineProperty;
        GraphicsTextProperty        m_textProperty;
};

using GraphicsContextPtr = std::shared_ptr<CGraphicsContext>;

/** @endcond */

#endif // CGLOBALGRAPHICSCONTEXT_H
