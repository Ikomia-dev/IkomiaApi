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

#ifndef COCVWIDGETFAST_HPP
#define COCVWIDGETFAST_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvFAST.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetFAST : public CWorkflowTaskWidget
{
    public:

        COcvWidgetFAST(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetFAST(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvFASTParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFASTParam>();

            m_pSpinThresh = addSpin(0, tr("Threshold"), m_pParam->m_threshold, 0, INT_MAX);
            m_pCombo = addCombo(1, tr("Type"));
            m_pCombo->addItem("TYPE_5_8", cv::FastFeatureDetector::TYPE_5_8);
            m_pCombo->addItem("TYPE_7_12", cv::FastFeatureDetector::TYPE_7_12);
            m_pCombo->addItem("TYPE_9_16", cv::FastFeatureDetector::TYPE_9_16);
            m_pCombo->setCurrentIndex(m_pCombo->findData(m_pParam->m_type));
            m_pCheck = addCheck(2, tr("Non maximum suppression"), m_pParam->m_bNonmaxSuppression);
        }

        void onApply() override
        {
            m_pParam->m_threshold = m_pSpinThresh->value();
            m_pParam->m_type = m_pCombo->currentData().toInt();
            m_pParam->m_bNonmaxSuppression = m_pCheck->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvFASTParam> m_pParam = nullptr;
        QSpinBox*   m_pSpinThresh = nullptr;
        QComboBox*  m_pCombo = nullptr;
        QCheckBox*  m_pCheck = nullptr;
};

class COcvWidgetFASTFactory : public CWidgetFactory
{
    public:

        COcvWidgetFASTFactory()
        {
            m_name = "ocv_fast";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFAST>(pParam);
        }
};
#endif // COCVWIDGETFAST_HPP
