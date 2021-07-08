/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CGRAPHICSOUTPUT_H
#define CGRAPHICSOUTPUT_H

#include "DataProcessGlobal.hpp"
#include "Graphics/CGraphicsLayer.h"
#include "Workflow/CWorkflowTaskIO.h"

/** @cond INTERNAL */

//class CGraphicsLayer;
class QGraphicsItem;
class CGraphicsInput;

//CGraphicsOutput handle CProxyGraphicsItem only for
//thread safety. Several Qt-based graphics (QGraphicsText) must be instanciate in the main thread
//The use of CProxyGraphicsItem also facilitate Python bindings
class DATAPROCESSSHARED_EXPORT CGraphicsOutput : public CWorkflowTaskIO
{
    public:

        using GraphicsOutputPtr = std::shared_ptr<CGraphicsOutput>;

        CGraphicsOutput();
        CGraphicsOutput(const CGraphicsOutput& out);
        CGraphicsOutput(CGraphicsOutput&& out);
        CGraphicsOutput(const CGraphicsInput& in);

        virtual ~CGraphicsOutput();

        CGraphicsOutput&     operator=(const CGraphicsOutput& out);
        CGraphicsOutput&     operator=(CGraphicsOutput&& out);
        CGraphicsOutput&     operator=(const CGraphicsInput& in);

        void                        setNewLayer(const std::string name);
        void                        setImageIndex(int index);
        void                        setItems(const std::vector<ProxyGraphicsItemPtr>& items);

        int                                 getImageIndex() const;
        std::vector<ProxyGraphicsItemPtr>   getItems() const;

        bool                        isDataAvailable() const override;

        CGraphicsLayer*             createLayer(const GraphicsContextPtr &globalContext);

        void                        addItem(const ProxyGraphicsItemPtr& itemPtr);
        ProxyGraphicsItemPtr        addPoint(const CPointF& pt);
        ProxyGraphicsItemPtr        addPoint(const CPointF& pt, const CGraphicsPointProperty& prop);
        ProxyGraphicsItemPtr        addRectangle(float x, float y, float width, float height);
        ProxyGraphicsItemPtr        addRectangle(float x, float y, float width, float height, const CGraphicsRectProperty& prop);
        ProxyGraphicsItemPtr        addEllipse(float x, float y, float width, float height);
        ProxyGraphicsItemPtr        addEllipse(float x, float y, float width, float height, const CGraphicsEllipseProperty& prop);
        ProxyGraphicsItemPtr        addPolygon(const std::vector<CPointF> &points);
        ProxyGraphicsItemPtr        addPolygon(const std::vector<CPointF> &points, const CGraphicsPolygonProperty& prop);
        ProxyGraphicsItemPtr        addPolyline(const std::vector<CPointF> &points);
        ProxyGraphicsItemPtr        addPolyline(const std::vector<CPointF> &points, const CGraphicsPolylineProperty& prop);
        ProxyGraphicsItemPtr        addComplexPolygon(const PolygonF& outer, const std::vector<PolygonF>& inners);
        ProxyGraphicsItemPtr        addComplexPolygon(const PolygonF& outer, const std::vector<PolygonF>& inners, const CGraphicsPolygonProperty& prop);
        ProxyGraphicsItemPtr        addText(const std::string& text, float x, float y);
        ProxyGraphicsItemPtr        addText(const std::string& text, float x, float y, const CGraphicsTextProperty& prop);

        void                        clearData() override;

        void                        copy(const WorkflowTaskIOPtr &ioPtr) override;

        GraphicsOutputPtr           clone() const;

        void                        save() override;
        void                        save(const std::string &path) override;

    private:

        virtual WorkflowTaskIOPtr   cloneImp() const override;

    private:

        std::string                         m_layerName;
        std::vector<ProxyGraphicsItemPtr>   m_items;
        int                                 m_imageIndex = 0;
};

using GraphicsOutputPtr = std::shared_ptr<CGraphicsOutput>;

class DATAPROCESSSHARED_EXPORT CGraphicsOutputFactory: public CWorkflowTaskIOFactory
{
    public:

        CGraphicsOutputFactory()
        {
            m_name = "CGraphicsOutput";
        }

        virtual WorkflowTaskIOPtr   create(IODataType dataType)
        {
            Q_UNUSED(dataType);
            return std::make_shared<CGraphicsOutput>();
        }
};

/** @endcond */

#endif // CGRAPHICSOUTPUT_H
