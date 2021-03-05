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

#include <QObject>
#include "CFolderInput.h"

CFolderInput::CFolderInput() : CProtocolTaskIO(IODataType::FOLDER)
{
    m_description = QObject::tr("File system path (folder only).\nUsed as input only.").toStdString();
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

CFolderInput::CFolderInput(const std::string &path) : CProtocolTaskIO(IODataType::FOLDER)
{
    m_description = QObject::tr("File system path (folder only).\nUsed as input only.").toStdString();
    m_path = path;
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

void CFolderInput::setPath(const std::string &path)
{
    assert(m_infoPtr);
    m_path = path;
    m_infoPtr->setFileName(path);
}

std::string CFolderInput::getPath() const
{
    return m_path;
}

bool CFolderInput::isDataAvailable() const
{
    return true;
}

void CFolderInput::clearData()
{
    m_path.clear();
}

std::shared_ptr<CProtocolTaskIO> CFolderInput::clone() const
{
    return std::static_pointer_cast<CFolderInput>(cloneImp());
}

std::shared_ptr<CProtocolTaskIO> CFolderInput::cloneImp() const
{
    return std::shared_ptr<CFolderInput>(new CFolderInput(*this));
}
