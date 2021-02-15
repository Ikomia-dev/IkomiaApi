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

#include "CWidgetOutput.h"
#include <QPainter>
#include "Main/CoreTools.hpp"

CWidgetOutput::CWidgetOutput() : CProtocolTaskIO(IODataType::WIDGET)
{
    m_name = "CWidgetOutput";
    m_description = QObject::tr("Generic widget for custom display.\nAny Qt-based window (ex: Matplotlib canvas).").toStdString();
    m_saveFormat = DataFileFormat::JPG;
}

CWidgetOutput::CWidgetOutput(IODataType data): CProtocolTaskIO(data)
{
    m_name = "CWidgetOutput";
    m_description = QObject::tr("Generic widget for custom display.\nAny Qt-based window (ex: Matplotlib canvas).").toStdString();
    m_saveFormat = DataFileFormat::JPG;
}

CWidgetOutput::~CWidgetOutput()
{
    if(m_pWidget)
        delete m_pWidget;
}

void CWidgetOutput::setWidget(QWidget *pWidget)
{
    m_pWidget = pWidget;
}

QWidget *CWidgetOutput::getWidget() const
{
    return m_pWidget;
}

bool CWidgetOutput::isDataAvailable() const
{
    return m_pWidget != nullptr;
}

void CWidgetOutput::clearData()
{
    m_pWidget = nullptr;
}

void CWidgetOutput::save()
{
    std::string path = m_saveFolder + m_saveBaseName + Utils::Data::getFileFormatExtension(m_saveFormat);
    save(path);
}

void CWidgetOutput::save(const std::string &path)
{
    if(!m_pWidget)
        return;

    QImage img(m_pWidget->size(), QImage::Format_RGB32);
    QPainter painter(&img);
    m_pWidget->render(&painter);
    img.save(QString::fromStdString(path));
}

std::shared_ptr<CWidgetOutput> CWidgetOutput::clone() const
{
    return std::static_pointer_cast<CWidgetOutput>(cloneImp());
}

std::shared_ptr<CProtocolTaskIO> CWidgetOutput::cloneImp() const
{
    return std::shared_ptr<CWidgetOutput>(new CWidgetOutput(*this));
}
