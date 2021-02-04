#ifndef CGRAPHICSPROCESSIO_H
#define CGRAPHICSPROCESSIO_H

#include "DataProcessGlobal.hpp"
#include "Graphics/CGraphicsLayer.h"
#include "Protocol/CProtocolTaskIO.h"

/** @cond INTERNAL */

//class CGraphicsLayer;
class QGraphicsItem;
class CGraphicsProcessInput;

//CGraphicsProcessOutput handle CProxyGraphicsItem only for
//thread safety. Several Qt-based graphics (QGraphicsText) must be instanciate in the main thread
//The use of CProxyGraphicsItem also facilitate Python bindings
class DATAPROCESSSHARED_EXPORT CGraphicsProcessOutput : public CProtocolTaskIO
{
    public:

        using GraphicsProcessOutputPtr = std::shared_ptr<CGraphicsProcessOutput>;

        CGraphicsProcessOutput();
        CGraphicsProcessOutput(const CGraphicsProcessOutput& out);
        CGraphicsProcessOutput(CGraphicsProcessOutput&& out);
        CGraphicsProcessOutput(const CGraphicsProcessInput& in);

        virtual ~CGraphicsProcessOutput();

        CGraphicsProcessOutput&     operator=(const CGraphicsProcessOutput& out);
        CGraphicsProcessOutput&     operator=(CGraphicsProcessOutput&& out);
        CGraphicsProcessOutput&     operator=(const CGraphicsProcessInput& in);

        void                        setNewLayer(const std::string name);
        void                        setImageIndex(int index);

        int                                 getImageIndex() const;
        std::vector<ProxyGraphicsItemPtr>   getItems() const;

        bool                        isDataAvailable() const override;

        CGraphicsLayer*             createLayer(const GraphicsContextPtr &globalContext);

        void                        addItem(const ProxyGraphicsItemPtr& itemPtr);
        ProxyGraphicsItemPtr        addPoint(const CPointF& pt);
        ProxyGraphicsItemPtr        addPoint(const CPointF& pt, const GraphicsPointProperty& prop);
        ProxyGraphicsItemPtr        addRectangle(float x, float y, float width, float height);
        ProxyGraphicsItemPtr        addRectangle(float x, float y, float width, float height, const GraphicsRectProperty& prop);
        ProxyGraphicsItemPtr        addEllipse(float x, float y, float width, float height);
        ProxyGraphicsItemPtr        addEllipse(float x, float y, float width, float height, const GraphicsEllipseProperty& prop);
        ProxyGraphicsItemPtr        addPolygon(const std::vector<CPointF> &points);
        ProxyGraphicsItemPtr        addPolygon(const std::vector<CPointF> &points, const GraphicsPolygonProperty& prop);
        ProxyGraphicsItemPtr        addPolyline(const std::vector<CPointF> &points);
        ProxyGraphicsItemPtr        addPolyline(const std::vector<CPointF> &points, const GraphicsPolylineProperty& prop);
        ProxyGraphicsItemPtr        addComplexPolygon(const PolygonF& outer, const std::vector<PolygonF>& inners);
        ProxyGraphicsItemPtr        addComplexPolygon(const PolygonF& outer, const std::vector<PolygonF>& inners, const GraphicsPolygonProperty& prop);
        ProxyGraphicsItemPtr        addText(const std::string& text, float x, float y);
        ProxyGraphicsItemPtr        addText(const std::string& text, float x, float y, const GraphicsTextProperty& prop);

        void                        clearData() override;

        void                        copy(const ProtocolTaskIOPtr &ioPtr) override;

        GraphicsProcessOutputPtr    clone() const;

        void                        save() override;
        void                        save(const std::string &path) override;

    private:

        virtual ProtocolTaskIOPtr   cloneImp() const override;

    private:

        std::string                         m_layerName;
        std::vector<ProxyGraphicsItemPtr>   m_items;
        int                                 m_imageIndex = 0;
};

class DATAPROCESSSHARED_EXPORT CGraphicsProcessOutputFactory: public CProtocolTaskIOFactory
{
    public:

        CGraphicsProcessOutputFactory()
        {
            m_name = "CGraphicsProcessOutput";
        }

        virtual ProtocolTaskIOPtr   create(IODataType dataType)
        {
            Q_UNUSED(dataType);
            return std::make_shared<CGraphicsProcessOutput>();
        }
};

/** @endcond */

#endif // CGRAPHICSPROCESSIO_H
