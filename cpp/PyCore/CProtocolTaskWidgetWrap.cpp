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

#include "CProtocolTaskWidgetWrap.h"
#include <QLayout>
#include <QDebug>

CProtocolTaskWidgetWrap::CProtocolTaskWidgetWrap() : CProtocolTaskWidget(nullptr)
{
}

CProtocolTaskWidgetWrap::CProtocolTaskWidgetWrap(QWidget *parent) : CProtocolTaskWidget(parent)
{
}

void CProtocolTaskWidgetWrap::init()
{
    CPyEnsureGIL gil;
    try
    {
        this->get_override("init")();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::onApply()
{
    CPyEnsureGIL gil;
    try
    {
        if(override onApplyOver = this->get_override("onApply"))
            onApplyOver();
        else
            CProtocolTaskWidget::onApply();
    }
    catch(boost::python::error_already_set&)
    {
        //Do not throw exceptions from slot
        qCritical().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CProtocolTaskWidgetWrap::default_onApply()
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTaskWidget::onApply();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::setLayout(long long layoutPtr)
{
    CPyEnsureGIL gil;
    try
    {
        auto pLayout = reinterpret_cast<QLayout*>(layoutPtr);
        m_pLayout->addLayout(pLayout, 0, 0);
        //this->CProtocolTaskWidget::setLayout(pLayout);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::emitApply(const ProtocolTaskParamPtr& paramPtr)
{
    try
    {
        emit doApplyProcess(paramPtr);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::emitSendProcessAction(int flags)
{
    try
    {
        emit doSendProcessAction(flags);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::emitSetGraphicsTool(GraphicsShape tool)
{
    try
    {
        emit doSetGraphicsTool(tool);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::emitSetGraphicsCategory(const std::string &category)
{
    try
    {
        emit doSetGraphicsCategory(QString::fromStdString(category));
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

