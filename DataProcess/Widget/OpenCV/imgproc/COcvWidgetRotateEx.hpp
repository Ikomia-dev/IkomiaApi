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

#ifndef COCVWIDGETROTATEEX_HPP
#define COCVWIDGETROTATEEX_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvRotateEx.hpp"
#include <QDoubleSpinBox>

class COcvWidgetRotateEx : public CProtocolTaskWidget
{
    public:

        COcvWidgetRotateEx(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetRotateEx(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvRotateExParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvRotateExParam>();

            auto pLabelAngle = new QLabel(tr("Angle"));
            m_pSpinAngle = new QDoubleSpinBox;
            m_pSpinAngle->setRange(0, 360);
            m_pSpinAngle->setSingleStep(1.0);
            m_pSpinAngle->setValue(-m_pParam->m_angle);

            auto pLabelScale = new QLabel(tr("Scale"));
            m_pSpinScale = new QDoubleSpinBox;
            m_pSpinScale->setRange(0, 100);
            m_pSpinScale->setSingleStep(1.0);
            m_pSpinScale->setValue(m_pParam->m_scale);

            auto pLabelInterpolation = new QLabel(tr("Interpolation"));
            m_pComboInterpolation = new QComboBox;
            m_pComboInterpolation->addItem(tr("Nearest neighbor"), cv::INTER_NEAREST);
            m_pComboInterpolation->addItem(tr("Bilinear"), cv::INTER_LINEAR);
            m_pComboInterpolation->addItem(tr("Cubic"), cv::INTER_CUBIC);
            m_pComboInterpolation->addItem(tr("Area"), cv::INTER_AREA);
            m_pComboInterpolation->addItem(tr("Lanczos 4"), cv::INTER_LANCZOS4);
            m_pComboInterpolation->addItem(tr("Exact bilinear"), cv::INTER_LINEAR_EXACT);
            m_pComboInterpolation->setCurrentIndex(m_pComboInterpolation->findData(m_pParam->m_interpolation));

            
            m_pLayout->addWidget(pLabelAngle, 0, 0);
            m_pLayout->addWidget(m_pSpinAngle, 0, 1);
            m_pLayout->addWidget(pLabelScale, 1, 0);
            m_pLayout->addWidget(m_pSpinScale, 1, 1);
            m_pLayout->addWidget(pLabelInterpolation, 2, 0);
            m_pLayout->addWidget(m_pComboInterpolation, 2, 1);
            

            initConnections();
        }

        void initConnections()
        {
            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_angle = -m_pSpinAngle->value();
                m_pParam->m_scale = m_pSpinScale->value();
                m_pParam->m_interpolation = m_pComboInterpolation->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvRotateExParam>  m_pParam = nullptr;
        QDoubleSpinBox*                     m_pSpinAngle = nullptr;
        QDoubleSpinBox*                     m_pSpinScale = nullptr;
        QComboBox*                          m_pComboInterpolation = nullptr;
};

class COcvWidgetRotateExFactory : public CWidgetFactory
{
    public:

        COcvWidgetRotateExFactory()
        {
            m_name = QObject::tr("RotateEx").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetRotateEx>(pParam);
        }
};

#endif // COCVWIDGETROTATEEX_HPP
