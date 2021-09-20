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

#ifndef COCVWIDGETNIBLACKTHRESHOLD_HPP
#define COCVWIDGETNIBLACKTHRESHOLD_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvNiblackThreshold.hpp"

class COcvWidgetNiblackThreshold : public CWorkflowTaskWidget
{
    public:

        COcvWidgetNiblackThreshold(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetNiblackThreshold(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvNiblackThresholdParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvNiblackThresholdParam>();

            auto pLabelMethod = new QLabel(QObject::tr("Binarization method"));
            m_pComboMethod = new QComboBox;
            m_pComboMethod->addItem(tr("Niblack"), cv::ximgproc::BINARIZATION_NIBLACK);
            m_pComboMethod->addItem(tr("Sauvola"), cv::ximgproc::BINARIZATION_SAUVOLA);
            m_pComboMethod->addItem(tr("Wolf"), cv::ximgproc::BINARIZATION_WOLF);
            m_pComboMethod->addItem(tr("Nick"), cv::ximgproc::BINARIZATION_NICK);
            m_pComboMethod->setCurrentIndex(m_pComboMethod->findData(m_pParam->m_binaryMethod));

            auto pLabelThreshType = new QLabel(QObject::tr("Threshold type"));
            m_pComboThreshType = new QComboBox;
            m_pComboThreshType->addItem(tr("Binary"), cv::THRESH_BINARY);
            m_pComboThreshType->addItem(tr("Inverse binary"), cv::THRESH_BINARY_INV);
            m_pComboThreshType->setCurrentIndex(m_pComboMethod->findData(m_pParam->m_thresholdType));

            auto pLabelBlockSize = new QLabel(QObject::tr("Block size"));
            m_pSpinBlockSize = new QDoubleSpinBox;
            m_pSpinBlockSize->setSingleStep(2);
            m_pSpinBlockSize->setRange(1, INT_MAX - 1);
            m_pSpinBlockSize->setValue(m_pParam->m_blockSize);

            auto pLabelK = new QLabel(QObject::tr("K"));
            m_pSpinK = new QDoubleSpinBox;
            m_pSpinK->setValue(m_pParam->m_k);
            m_pSpinK->setRange(-1.0, 1.0);
            m_pSpinK->setSingleStep(0.1);
            
            m_pLayout->addWidget(pLabelMethod, 0, 0);
            m_pLayout->addWidget(m_pComboMethod, 0, 1);
            m_pLayout->addWidget(pLabelThreshType, 1, 0);
            m_pLayout->addWidget(m_pComboThreshType, 1, 1);
            m_pLayout->addWidget(pLabelBlockSize, 2, 0);
            m_pLayout->addWidget(m_pSpinBlockSize, 2, 1);
            m_pLayout->addWidget(pLabelK, 3, 0);
            m_pLayout->addWidget(m_pSpinK, 3, 1);
        }

        void onApply() override
        {
            m_pParam->m_binaryMethod = m_pComboMethod->currentData().toInt();
            m_pParam->m_thresholdType = m_pComboThreshType->currentData().toInt();
            m_pParam->m_blockSize = m_pSpinBlockSize->value();
            m_pParam->m_k = m_pSpinK->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvNiblackThresholdParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinBlockSize = nullptr;
        QDoubleSpinBox* m_pSpinK = nullptr;
        QComboBox*      m_pComboMethod = nullptr;
        QComboBox*      m_pComboThreshType = nullptr;
};

class COcvWidgetNiblackThresholdFactory : public CWidgetFactory
{
    public:

        COcvWidgetNiblackThresholdFactory()
        {
            m_name = "ocv_niblack_threshold";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetNiblackThreshold>(pParam);
        }
};

#endif // COCVWIDGETNIBLACKTHRESHOLD_HPP
