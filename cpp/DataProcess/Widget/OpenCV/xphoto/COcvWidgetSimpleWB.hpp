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

#ifndef COCVWIDGETSIMPLEWB_HPP
#define COCVWIDGETSIMPLEWB_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/xphoto/COcvSimpleWB.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetSimpleWB : public CProtocolTaskWidget
{
    public:

        COcvWidgetSimpleWB(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetSimpleWB(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSimpleWBParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSimpleWBParam>();

            auto pSpinInMin = addDoubleSpin(0, tr("Input min"), m_pParam->m_inputMin);
            auto pSpinInMax = addDoubleSpin(1, tr("Input max"), m_pParam->m_inputMax);
            auto pSpinOutMin = addDoubleSpin(2, tr("Output min"), m_pParam->m_outputMin);
            auto pSpinOutMax = addDoubleSpin(3, tr("Output max"), m_pParam->m_outputMax);
            auto pSpinP = addDoubleSpin(4, tr("% of top/bottom to ignore"), m_pParam->m_P);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_inputMin = pSpinInMin->value();
                m_pParam->m_inputMax = pSpinInMax->value();
                m_pParam->m_outputMin = pSpinOutMin->value();
                m_pParam->m_outputMax = pSpinOutMax->value();
                m_pParam->m_P = pSpinP->value();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvSimpleWBParam>   m_pParam = nullptr;
        QDoubleSpinBox*                         m_pDblSpinSat = nullptr;
};

class COcvWidgetSimpleWBFactory : public CWidgetFactory
{
    public:

        COcvWidgetSimpleWBFactory()
        {
            m_name = QObject::tr("SimpleWB").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSimpleWB>(pParam);
        }
};
#endif // COCVWIDGETSIMPLEWB_HPP
