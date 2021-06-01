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

#ifndef COCVWIDGETPEILINNORMALIZATION_HPP
#define COCVWIDGETPEILINNORMALIZATION_HPP

#include "Process/OpenCV/ximgproc/COcvPeiLinNormalization.hpp"

class COcvWidgetPeiLinNormalization : public CProtocolTaskWidget
{
    public:

        COcvWidgetPeiLinNormalization(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetPeiLinNormalization(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvPeiLinNormalizationParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvPeiLinNormalizationParam>();

            auto pLabelInterpolation = new QLabel(tr("Interpolation"));
            auto pComboInterpolation = new QComboBox;
            pComboInterpolation->addItem(tr("Nearest neighbor"), cv::INTER_NEAREST);
            pComboInterpolation->addItem(tr("Bilinear"), cv::INTER_LINEAR);
            pComboInterpolation->addItem(tr("Cubic"), cv::INTER_CUBIC);
            pComboInterpolation->addItem(tr("Area"), cv::INTER_AREA);
            pComboInterpolation->addItem(tr("Lanczos 4"), cv::INTER_LANCZOS4);
            pComboInterpolation->addItem(tr("Exact bilinear"), cv::INTER_LINEAR_EXACT);
            pComboInterpolation->setCurrentIndex(pComboInterpolation->findData(m_pParam->m_interpolation));

            
            m_pLayout->addWidget(pLabelInterpolation, 0, 0);
            m_pLayout->addWidget(pComboInterpolation, 0, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_interpolation = pComboInterpolation->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvPeiLinNormalizationParam>  m_pParam = nullptr;
};

class COcvWidgetPeiLinNormalizationFactory : public CWidgetFactory
{
    public:

        COcvWidgetPeiLinNormalizationFactory()
        {
            m_name = QObject::tr("Pei-Lin Normalization").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetPeiLinNormalization>(pParam);
        }
};

#endif // COCVWIDGETPEILINNORMALIZATION_HPP
