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

#ifndef COCVWIDGETNORMALIZE_HPP
#define COCVWIDGETNORMALIZE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvNormalize.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetNormalize : public CWorkflowTaskWidget
{
    public:

        COcvWidgetNormalize(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetNormalize(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvNormalizeParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvNormalizeParam>();

            m_pSpinAlpha = addDoubleSpin(0, tr("Alpha"), m_pParam->m_alpha);
            m_pSpinBeta = addDoubleSpin(1, tr("Beta"), m_pParam->m_beta);
            m_pCombo = addCombo(2, tr("Norm type"));
            m_pCombo->addItem(tr("NORM_INF"), cv::NORM_INF);
            m_pCombo->addItem(tr("NORM_L1"), cv::NORM_L1);
            m_pCombo->addItem(tr("NORM_L2"), cv::NORM_L2);
            m_pCombo->addItem(tr("NORM_L2SQR"), cv::NORM_L2SQR);
            m_pCombo->addItem(tr("NORM_HAMMING"), cv::NORM_HAMMING);
            m_pCombo->addItem(tr("NORM_HAMMING2"), cv::NORM_HAMMING2);
            m_pCombo->addItem(tr("NORM_TYPE_MASK"), cv::NORM_TYPE_MASK);
            m_pCombo->addItem(tr("NORM_MINMAX"), cv::NORM_MINMAX);
            m_pCombo->addItem(tr("NORM_RELATIVE"), cv::NORM_RELATIVE);
            m_pCombo->setCurrentIndex(m_pCombo->findData(m_pParam->m_norm_type));

            m_pComboDepth = addCombo(3, tr("Depth"));
            m_pComboDepth->addItem("Default", -1);
            m_pComboDepth->addItem("CV_8U", CV_8U);
            m_pComboDepth->addItem("CV_16U", CV_16U);
            m_pComboDepth->addItem("CV_32F", CV_32F);
            m_pComboDepth->addItem("CV_64F", CV_64F);
            m_pComboDepth->setCurrentIndex(m_pComboDepth->findData(m_pParam->m_dtype));
        }

        void onApply() override
        {
            m_pParam->m_alpha = m_pSpinAlpha->value();
            m_pParam->m_beta = m_pSpinBeta->value();
            m_pParam->m_norm_type = m_pCombo->currentData().toInt();
            m_pParam->m_dtype = m_pComboDepth->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvNormalizeParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinAlpha = nullptr;
        QDoubleSpinBox* m_pSpinBeta = nullptr;
        QComboBox* m_pCombo = nullptr;
        QComboBox* m_pComboDepth = nullptr;
};

class COcvWidgetNormalizeFactory : public CWidgetFactory
{
    public:

        COcvWidgetNormalizeFactory()
        {
            m_name = "ocv_normalize";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetNormalize>(pParam);
        }
};
#endif // COCVWIDGETNORMALIZE_HPP
