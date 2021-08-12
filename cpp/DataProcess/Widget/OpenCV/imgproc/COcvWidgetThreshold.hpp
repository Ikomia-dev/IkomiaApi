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

#ifndef COCVWIDGETTHRESHOLD_HPP
#define COCVWIDGETTHRESHOLD_HPP

#include <QComboBox>
#include <QDoubleSpinBox>
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvThreshold.hpp"

class COcvWidgetThreshold : public CWorkflowTaskWidget
{
    public:

        COcvWidgetThreshold(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetThreshold(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvThresholdParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvThresholdParam>();

            m_pComboMethod = addCombo(0, tr("Threshold method"));
            m_pComboMethod->addItem(tr("Fixed value"), 0);
            m_pComboMethod->addItem(tr("Otsu"), cv::THRESH_OTSU);
            m_pComboMethod->addItem(tr("Triangle"), cv::THRESH_TRIANGLE);

            if(m_pParam->m_thresholdType & cv::THRESH_OTSU)
                m_pComboMethod->setCurrentIndex(1);
            else if(m_pParam->m_thresholdType & cv::THRESH_TRIANGLE)
                m_pComboMethod->setCurrentIndex(2);
            else
                m_pComboMethod->setCurrentIndex(0);

            m_pComboType = addCombo(1, tr("Threshold type"));
            m_pComboType->addItem(tr("Binary"), cv::THRESH_BINARY);
            m_pComboType->addItem(tr("Inverse binary"), cv::THRESH_BINARY_INV);
            m_pComboType->addItem(tr("Truncate"), cv::THRESH_TRUNC);
            m_pComboType->addItem(tr("To zero"), cv::THRESH_TOZERO);
            m_pComboType->addItem(tr("To zero inverse"), cv::THRESH_TOZERO_INV);

            if(m_pParam->m_thresholdType == cv::THRESH_BINARY || m_pParam->m_thresholdType == cv::THRESH_OTSU || m_pParam->m_thresholdType == cv::THRESH_TRIANGLE)
                m_pComboType->setCurrentIndex(0);
            else if(m_pParam->m_thresholdType & cv::THRESH_BINARY_INV)
                m_pComboType->setCurrentIndex(1);
            else if(m_pParam->m_thresholdType & cv::THRESH_TRUNC)
                m_pComboType->setCurrentIndex(2);
            else if(m_pParam->m_thresholdType & cv::THRESH_TOZERO)
                m_pComboType->setCurrentIndex(3);
            else
                m_pComboType->setCurrentIndex(4);

            m_pSpinValue = addDoubleSpin(2, tr("Threshold value"), m_pParam->m_threshold);
        }

        void onApply() override
        {
            m_pParam->m_thresholdType = m_pComboMethod->currentData().toInt() | m_pComboType->currentData().toInt();
            m_pParam->m_threshold = m_pSpinValue->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvThresholdParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinValue = nullptr;
        QComboBox*      m_pComboMethod = nullptr;
        QComboBox*      m_pComboType = nullptr;
};

class COcvWidgetThresholdFactory : public CWidgetFactory
{
    public:

        COcvWidgetThresholdFactory()
        {
            m_name = QObject::tr("Threshold").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetThreshold>(pParam);
        }
};

#endif // COCVWIDGETTHRESHOLD_HPP
