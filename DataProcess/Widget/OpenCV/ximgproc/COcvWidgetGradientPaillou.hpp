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

#ifndef COCVWIDGETGRADIENTPAILLOU_HPP
#define COCVWIDGETGRADIENTPAILLOU_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvGradientPaillou.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>

class COcvWidgetGradientPaillou : public CProtocolTaskWidget
{
    public:

        COcvWidgetGradientPaillou(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetGradientPaillou(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGradientPaillouParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGradientPaillouParam>();

            QGroupBox* pBox = new QGroupBox("Orientation");
            QGridLayout* pBoxLayout = new QGridLayout;
            QRadioButton* pRadioX = new QRadioButton("X");
            QRadioButton* pRadioY = new QRadioButton("Y");
            pBoxLayout->addWidget(pRadioX);
            pBoxLayout->addWidget(pRadioY);
            pBox->setLayout(pBoxLayout);
            if(m_pParam->m_orientation == COcvGradientPaillouParam::orientation::X)
                pRadioX->setChecked(true);
            else
                pRadioY->setChecked(true);

            QLabel* pLabelSpinAlpha = new QLabel(QObject::tr("Alpha"));
            m_pDblSpinAlpha = new QDoubleSpinBox;
            m_pDblSpinAlpha->setRange(0, 4);
            m_pDblSpinAlpha->setSingleStep(0.1);
            m_pDblSpinAlpha->setValue(m_pParam->m_alpha);

            QLabel* pLabelSpinOmega = new QLabel(QObject::tr("Omega"));
            m_pDblSpinOmega = new QDoubleSpinBox;
            m_pDblSpinOmega->setRange(0, 4);
            m_pDblSpinOmega->setSingleStep(0.1);
            m_pDblSpinOmega->setValue(m_pParam->m_omega);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_orientation = pRadioX->isChecked() ? COcvGradientPaillouParam::orientation::X : COcvGradientPaillouParam::orientation::Y;
                m_pParam->m_alpha = m_pDblSpinAlpha->value();
                m_pParam->m_omega = m_pDblSpinOmega->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pBox, 0, 0, 1, 2);

            m_pLayout->addWidget(pLabelSpinAlpha, 1, 0);
            m_pLayout->addWidget(m_pDblSpinAlpha, 1, 1);

            m_pLayout->addWidget(pLabelSpinOmega, 2, 0);
            m_pLayout->addWidget(m_pDblSpinOmega, 2, 1);

            
        }

    private:

        std::shared_ptr<COcvGradientPaillouParam> m_pParam = nullptr;
        QDoubleSpinBox*                     m_pDblSpinAlpha = nullptr;
        QDoubleSpinBox*                     m_pDblSpinOmega = nullptr;
};

class COcvWidgetGradientPaillouFactory : public CWidgetFactory
{
    public:

        COcvWidgetGradientPaillouFactory()
        {
            m_name = QObject::tr("Paillou gradient filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGradientPaillou>(pParam);
        }
};
#endif // COCVWIDGETGRADIENTPAILLOU_HPP
