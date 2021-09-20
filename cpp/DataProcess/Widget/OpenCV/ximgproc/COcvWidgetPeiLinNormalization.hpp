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

class COcvWidgetPeiLinNormalization : public CWorkflowTaskWidget
{
    public:

        COcvWidgetPeiLinNormalization(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetPeiLinNormalization(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
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
            m_pComboInterpolation = new QComboBox;
            m_pComboInterpolation->addItem(tr("Nearest neighbor"), cv::INTER_NEAREST);
            m_pComboInterpolation->addItem(tr("Bilinear"), cv::INTER_LINEAR);
            m_pComboInterpolation->addItem(tr("Cubic"), cv::INTER_CUBIC);
            m_pComboInterpolation->addItem(tr("Area"), cv::INTER_AREA);
            m_pComboInterpolation->addItem(tr("Lanczos 4"), cv::INTER_LANCZOS4);
            m_pComboInterpolation->addItem(tr("Exact bilinear"), cv::INTER_LINEAR_EXACT);
            m_pComboInterpolation->setCurrentIndex(m_pComboInterpolation->findData(m_pParam->m_interpolation));

            m_pLayout->addWidget(pLabelInterpolation, 0, 0);
            m_pLayout->addWidget(m_pComboInterpolation, 0, 1);
        }

        void onApply() override
        {
            m_pParam->m_interpolation = m_pComboInterpolation->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvPeiLinNormalizationParam>  m_pParam = nullptr;
        QComboBox*  m_pComboInterpolation = nullptr;
};

class COcvWidgetPeiLinNormalizationFactory : public CWidgetFactory
{
    public:

        COcvWidgetPeiLinNormalizationFactory()
        {
            m_name = "ocv_pei_lin_normalization";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetPeiLinNormalization>(pParam);
        }
};

#endif // COCVWIDGETPEILINNORMALIZATION_HPP
