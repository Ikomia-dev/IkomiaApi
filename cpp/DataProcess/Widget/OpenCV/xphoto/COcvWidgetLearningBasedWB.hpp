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

#ifndef COCVWIDGETLEARNINGBASEDWB_HPP
#define COCVWIDGETLEARNINGBASEDWB_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/xphoto/COcvLearningBasedWB.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetLearningBasedWB : public CWorkflowTaskWidget
{
    public:

        COcvWidgetLearningBasedWB(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetLearningBasedWB(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvLearningBasedWBParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvLearningBasedWBParam>();

            m_pSpinBin = addSpin(0, tr("Hist bin number"), m_pParam->m_histBinNum);
            m_pSpinMax = addSpin(1, tr("Max value"), m_pParam->m_rangeMaxVal);
            m_pSpinThresh = addDoubleSpin(2, tr("Saturation threshold"), m_pParam->m_satThreshold, 0, 1, 0.1);
        }

        void onApply() override
        {
            m_pParam->m_histBinNum = m_pSpinBin->value();
            m_pParam->m_rangeMaxVal = m_pSpinMax->value();
            m_pParam->m_satThreshold = m_pSpinThresh->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvLearningBasedWBParam>   m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinThresh = nullptr;
        QSpinBox*       m_pSpinBin = nullptr;
        QSpinBox*       m_pSpinMax = nullptr;
};

class COcvWidgetLearningBasedWBFactory : public CWidgetFactory
{
    public:

        COcvWidgetLearningBasedWBFactory()
        {
            m_name = "ocv_learning_based_wb";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetLearningBasedWB>(pParam);
        }
};

#endif // COCVWIDGETLEARNINGBASEDWB_HPP
