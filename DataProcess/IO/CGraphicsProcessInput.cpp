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

#include "CGraphicsProcessInput.h"
#include "Graphics/CGraphicsLayer.h"
#include "CGraphicsProcessOutput.h"

CGraphicsProcessInput::CGraphicsProcessInput() : CProtocolTaskIO(IODataType::INPUT_GRAPHICS)
{
    m_description = QObject::tr("Graphics items organized in layer.\n"
                                "Represent shapes and types of objects in image.\n"
                                "Graphics can be created interactively by user.").toStdString();
    m_saveFormat = DataFileFormat::JSON;
}

CGraphicsProcessInput::CGraphicsProcessInput(CGraphicsLayer *pLayer) : CProtocolTaskIO(IODataType::INPUT_GRAPHICS)
{
    m_description = QObject::tr("Graphics items organized in layer.\n"
                                "Represent shapes and types of objects in image.\n"
                                "Graphics can be created interactively by user.").toStdString();
    m_saveFormat = DataFileFormat::JSON;
    m_pLayer = pLayer;
    m_source = GraphicsSource::GRAPHICS_LAYER;
}

CGraphicsProcessInput::CGraphicsProcessInput(const CGraphicsProcessInput &in) : CProtocolTaskIO(in)
{
    m_source = in.m_source;
    m_pLayer = in.m_pLayer;
    m_items = in.m_items;
}

CGraphicsProcessInput::CGraphicsProcessInput(CGraphicsProcessInput &&in) : CProtocolTaskIO(in)
{
    m_source = std::move(in.m_source);
    m_pLayer = std::move(in.m_pLayer);
    m_items = std::move(in.m_items);
}

CGraphicsProcessInput::CGraphicsProcessInput(const CGraphicsProcessOutput &out) : CProtocolTaskIO(out)
{
    m_dataType = IODataType::INPUT_GRAPHICS;
    m_pLayer = nullptr;
    m_items = out.getItems();
    m_source = GraphicsSource::GRAPHICS_OUTPUT;
}

CGraphicsProcessInput &CGraphicsProcessInput::operator=(const CGraphicsProcessInput &in)
{
    CProtocolTaskIO::operator=(in);
    m_source = in.m_source;
    m_pLayer = in.m_pLayer;
    m_items = in.m_items;
    return *this;
}

CGraphicsProcessInput &CGraphicsProcessInput::operator=(CGraphicsProcessInput &&in)
{
    CProtocolTaskIO::operator=(in);
    m_source = std::move(in.m_source);
    m_pLayer = std::move(in.m_pLayer);
    m_items = std::move(in.m_items);
    return *this;
}

CGraphicsProcessInput &CGraphicsProcessInput::operator=(const CGraphicsProcessOutput &out)
{
    CProtocolTaskIO::operator=(out);
    m_dataType = IODataType::INPUT_GRAPHICS;
    m_pLayer = nullptr;
    m_items = out.getItems();
    m_source = GraphicsSource::GRAPHICS_OUTPUT;
    return *this;
}

void CGraphicsProcessInput::setLayer(CGraphicsLayer *pLayer)
{
    m_pLayer = pLayer;
    m_items.clear();
    m_source = GraphicsSource::GRAPHICS_LAYER;
}

void CGraphicsProcessInput::setItems(const std::vector<ProxyGraphicsItemPtr> &items)
{
    m_pLayer = nullptr;
    m_items = items;
    m_source = GraphicsSource::GRAPHICS_OUTPUT;
}

const CGraphicsLayer *CGraphicsProcessInput::getLayer() const
{
    return m_pLayer;
}

std::vector<ProxyGraphicsItemPtr> CGraphicsProcessInput::getItems() const
{
    if(m_source == GraphicsSource::GRAPHICS_LAYER && m_pLayer)
    {
        //Graphics from layer can be modified by user
        //so we retrieve a new list for each call
        std::vector<ProxyGraphicsItemPtr> items;
        auto graphicsItems = m_pLayer->getChildItems();

        for(int i=0; i<graphicsItems.size(); ++i)
        {
            auto pItem = dynamic_cast<CGraphicsItem*>(graphicsItems[i]);
            items.push_back(pItem->createProxyGraphicsItem());
        }
        return items;
    }
    else
        return m_items;
}

QRectF CGraphicsProcessInput::getBoundingRect() const
{
    QRectF rect;

    for(size_t i=0; i<m_items.size(); ++i)
        rect = rect.united(m_items[i]->getBoundingRect());

    return rect;
}

bool CGraphicsProcessInput::isDataAvailable() const
{
    if(m_source == GraphicsSource::GRAPHICS_LAYER)
        return m_pLayer != nullptr;
    else
        return m_items.size() > 0;
}

void CGraphicsProcessInput::clearData()
{
    m_pLayer = nullptr;
}

void CGraphicsProcessInput::copy(const std::shared_ptr<CProtocolTaskIO> &ioPtr)
{
    auto pGraphicsInput = dynamic_cast<const CGraphicsProcessInput*>(ioPtr.get());
    if(pGraphicsInput)
        *this = *pGraphicsInput;
    else
    {
        auto pGraphicsOutput = dynamic_cast<const CGraphicsProcessOutput*>(ioPtr.get());
        if(pGraphicsOutput)
            *this = *pGraphicsOutput;
    }
}

CGraphicsProcessInput::GraphicsProcessInputPtr CGraphicsProcessInput::clone() const
{
    return std::static_pointer_cast<CGraphicsProcessInput>(cloneImp());
}

std::shared_ptr<CProtocolTaskIO> CGraphicsProcessInput::cloneImp() const
{
    return std::shared_ptr<CGraphicsProcessInput>(new CGraphicsProcessInput(*this));
}
