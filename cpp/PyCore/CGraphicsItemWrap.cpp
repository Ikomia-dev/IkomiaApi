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

#include "CGraphicsItemWrap.h"

CGraphicsItemWrap::CGraphicsItemWrap() : CProxyGraphicsItem()
{
}

CGraphicsItemWrap::CGraphicsItemWrap(GraphicsItem type) : CProxyGraphicsItem(type)
{
}

void CGraphicsItemWrap::setCategory(const std::string &categ)
{
    CPyEnsureGIL gil;
    try
    {
        this->get_override("setCategory")(categ);
    }
    catch(boost::python::error_already_set&)
    {
        //Do not throw exceptions from slot
        Utils::print(Utils::Python::handlePythonException(), QtCriticalMsg);
    }
}

std::string CGraphicsItemWrap::getCategory() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->get_override("getCategory")();
    }
    catch(boost::python::error_already_set&)
    {
        //Do not throw exceptions from slot
        Utils::print(Utils::Python::handlePythonException(), QtCriticalMsg);
    }
}

QRectF CGraphicsItemWrap::getBoundingRect() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->get_override("getBoundingRect")();
    }
    catch(boost::python::error_already_set&)
    {
        //Do not throw exceptions from slot
        Utils::print(Utils::Python::handlePythonException(), QtCriticalMsg);
    }
}

bool CGraphicsItemWrap::isTextItem() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isTextItemOver = this->get_override("isTextItem"))
            return isTextItemOver();
        else
            return CProxyGraphicsItem::isTextItem();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CGraphicsItemWrap::default_isTextItem() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProxyGraphicsItem::isTextItem();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CGraphicsItemWrap::translate(float dx, float dy)
{
    CPyEnsureGIL gil;
    try
    {
        this->get_override("translate")(dx, dy);
    }
    catch(boost::python::error_already_set&)
    {
        //Do not throw exceptions from slot
        Utils::print(Utils::Python::handlePythonException(), QtCriticalMsg);
    }
}

void CGraphicsItemWrap::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    Q_UNUSED(image);
    Q_UNUSED(filler);
    Q_UNUSED(bForceFill);
    Q_UNUSED(bBinary);
}

std::shared_ptr<CProxyGraphicsItem> CGraphicsItemWrap::clone() const
{
    return nullptr;
}
