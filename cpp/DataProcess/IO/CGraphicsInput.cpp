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

#include "CGraphicsInput.h"
#include "Graphics/CGraphicsLayer.h"
#include "CGraphicsOutput.h"

CGraphicsInput::CGraphicsInput() : CWorkflowTaskIO(IODataType::INPUT_GRAPHICS, "CGraphicsInput")
{
    m_description = QObject::tr("Graphics items organized in layer.\n"
                                "Represent shapes and types of objects in image.\n"
                                "Graphics can be created interactively by user.").toStdString();
    m_saveFormat = DataFileFormat::JSON;
}

CGraphicsInput::CGraphicsInput(const std::string &name) : CWorkflowTaskIO(IODataType::INPUT_GRAPHICS, name)
{
    m_description = QObject::tr("Graphics items organized in layer.\n"
                                "Represent shapes and types of objects in image.\n"
                                "Graphics can be created interactively by user.").toStdString();
    m_saveFormat = DataFileFormat::JSON;
}

CGraphicsInput::CGraphicsInput(CGraphicsLayer *pLayer, const std::string &name) : CWorkflowTaskIO(IODataType::INPUT_GRAPHICS, name)
{
    m_description = QObject::tr("Graphics items organized in layer.\n"
                                "Represent shapes and types of objects in image.\n"
                                "Graphics can be created interactively by user.").toStdString();
    m_saveFormat = DataFileFormat::JSON;
    m_pLayer = pLayer;
    m_source = GraphicsSource::GRAPHICS_LAYER;
}

CGraphicsInput::CGraphicsInput(const CGraphicsInput &in) : CWorkflowTaskIO(in)
{
    m_source = in.m_source;
    m_pLayer = in.m_pLayer;
    m_items = in.m_items;
}

CGraphicsInput::CGraphicsInput(CGraphicsInput &&in) : CWorkflowTaskIO(in)
{
    m_source = std::move(in.m_source);
    m_pLayer = std::move(in.m_pLayer);
    m_items = std::move(in.m_items);
}

CGraphicsInput::CGraphicsInput(const CGraphicsOutput &out) : CWorkflowTaskIO(out)
{
    m_dataType = IODataType::INPUT_GRAPHICS;
    m_pLayer = nullptr;
    m_items = out.getItems();
    m_source = GraphicsSource::GRAPHICS_OUTPUT;
}

CGraphicsInput &CGraphicsInput::operator=(const CGraphicsInput &in)
{
    CWorkflowTaskIO::operator=(in);
    m_source = in.m_source;
    m_pLayer = in.m_pLayer;
    m_items = in.m_items;
    return *this;
}

CGraphicsInput &CGraphicsInput::operator=(CGraphicsInput &&in)
{
    CWorkflowTaskIO::operator=(in);
    m_source = std::move(in.m_source);
    m_pLayer = std::move(in.m_pLayer);
    m_items = std::move(in.m_items);
    return *this;
}

CGraphicsInput &CGraphicsInput::operator=(const CGraphicsOutput &out)
{
    CWorkflowTaskIO::operator=(out);
    m_dataType = IODataType::INPUT_GRAPHICS;
    m_pLayer = nullptr;
    m_items = out.getItems();
    m_source = GraphicsSource::GRAPHICS_OUTPUT;
    return *this;
}

void CGraphicsInput::setLayer(CGraphicsLayer *pLayer)
{
    m_pLayer = pLayer;
    m_items.clear();
    m_source = GraphicsSource::GRAPHICS_LAYER;
}

void CGraphicsInput::setItems(const std::vector<ProxyGraphicsItemPtr> &items)
{
    m_pLayer = nullptr;
    m_items = items;
    m_source = GraphicsSource::GRAPHICS_OUTPUT;
}

const CGraphicsLayer *CGraphicsInput::getLayer() const
{
    return m_pLayer;
}

std::vector<ProxyGraphicsItemPtr> CGraphicsInput::getItems() const
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

QRectF CGraphicsInput::getBoundingRect() const
{
    QRectF rect;

    for(size_t i=0; i<m_items.size(); ++i)
        rect = rect.united(m_items[i]->getBoundingQRect());

    return rect;
}

bool CGraphicsInput::isDataAvailable() const
{
    if(m_source == GraphicsSource::GRAPHICS_LAYER)
        return m_pLayer != nullptr;
    else
        return m_items.size() > 0;
}

void CGraphicsInput::clearData()
{
    m_pLayer = nullptr;
}

void CGraphicsInput::copy(const std::shared_ptr<CWorkflowTaskIO> &ioPtr)
{
    auto pGraphicsInput = dynamic_cast<const CGraphicsInput*>(ioPtr.get());
    if(pGraphicsInput)
        *this = *pGraphicsInput;
    else
    {
        auto pGraphicsOutput = dynamic_cast<const CGraphicsOutput*>(ioPtr.get());
        if(pGraphicsOutput)
            *this = *pGraphicsOutput;
    }
}

CGraphicsInput::GraphicsInputPtr CGraphicsInput::clone() const
{
    return std::static_pointer_cast<CGraphicsInput>(cloneImp());
}

std::shared_ptr<CWorkflowTaskIO> CGraphicsInput::cloneImp() const
{
    return std::shared_ptr<CGraphicsInput>(new CGraphicsInput(*this));
}
