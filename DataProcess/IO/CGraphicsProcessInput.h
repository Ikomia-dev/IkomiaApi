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

#ifndef CGRAPHICSPROCESSINPUT_H
#define CGRAPHICSPROCESSINPUT_H

#include "DataProcessGlobal.hpp"
#include "Protocol/CProtocolTaskIO.h"
#include "Graphics/CGraphicsItem.hpp"

/** @cond INTERNAL */

class CGraphicsLayer;
class QGraphicsItem;
class CGraphicsProcessOutput;

class DATAPROCESSSHARED_EXPORT CGraphicsProcessInput : public CProtocolTaskIO
{
    public:

        enum GraphicsSource { GRAPHICS_LAYER, GRAPHICS_OUTPUT };

        using GraphicsProcessInputPtr = std::shared_ptr<CGraphicsProcessInput>;

        CGraphicsProcessInput();
        CGraphicsProcessInput(CGraphicsLayer* pLayer);
        CGraphicsProcessInput(const CGraphicsProcessInput& in);
        CGraphicsProcessInput(CGraphicsProcessInput&& in);
        CGraphicsProcessInput(const CGraphicsProcessOutput &out);

        virtual ~CGraphicsProcessInput();

        CGraphicsProcessInput&      operator=(const CGraphicsProcessInput& in);
        CGraphicsProcessInput&      operator=(CGraphicsProcessInput&& in);
        CGraphicsProcessInput&      operator=(const CGraphicsProcessOutput& out);

        void                        setLayer(CGraphicsLayer* pLayer);
        void                        setItems(const std::vector<ProxyGraphicsItemPtr>& items);

        const CGraphicsLayer*               getLayer() const;
        std::vector<ProxyGraphicsItemPtr>   getItems() const;
        QRectF                              getBoundingRect() const;

        bool                        isDataAvailable() const override;

        void                        clearData() override;

        void                        copy(const std::shared_ptr<CProtocolTaskIO> &ioPtr) override;

        GraphicsProcessInputPtr     clone() const;

    private:

        virtual ProtocolTaskIOPtr   cloneImp() const override;

    private:

        //Must be seen as weak pointer
        const CGraphicsLayer*               m_pLayer = nullptr;
        std::vector<ProxyGraphicsItemPtr>   m_items;
        GraphicsSource                      m_source = GraphicsSource::GRAPHICS_LAYER;
};

using GraphicsProcessInputPtr = std::shared_ptr<CGraphicsProcessInput>;

class DATAPROCESSSHARED_EXPORT CGraphicsProcessInputFactory: public CProtocolTaskIOFactory
{
    public:

        CGraphicsProcessInputFactory()
        {
            m_name = "CGraphicsProcessInput";
        }

        virtual ProtocolTaskIOPtr   create(IODataType dataType)
        {
            Q_UNUSED(dataType);
            return std::make_shared<CGraphicsProcessInput>();
        }
};

/** @endcond */

#endif // CGRAPHICSPROCESSINPUT_H
