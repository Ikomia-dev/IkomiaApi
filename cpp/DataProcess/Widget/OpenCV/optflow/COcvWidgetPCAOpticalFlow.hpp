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

#ifndef COCVWIDGETPCAOPTICALFLOW_HPP
#define COCVWIDGETPCAOPTICALFLOW_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/optflow/COcvPCAOpticalFlow.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetPCAOF : public CWorkflowTaskWidget
{
    public:

        COcvWidgetPCAOF(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetPCAOF(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvPCAOFParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvPCAOFParam>();

            m_pSpinSizeW = addSpin(0, tr("Basis width"), m_pParam->m_basisSize.width);
            m_pSpinSizeH = addSpin(1, tr("Basis height"), m_pParam->m_basisSize.height);
            m_pSpinRate = addDoubleSpin(2, tr("Sparse rate"), m_pParam->m_sparseRate, 0, 1, 0.001, 3);
            m_pSpinCorners = addDoubleSpin(3, tr("Corners fraction"), m_pParam->m_retainedCornersFraction, 0, 1, 0.1, 1);
            m_pSpinOccTh = addDoubleSpin(4, tr("Occlusions threshold"), m_pParam->m_occlusionsThreshold, 0, 1, 0.0001, 4);
            m_pSpinDamp = addDoubleSpin(5, tr("Damping factor"), m_pParam->m_dampingFactor, 0, 1, 0.00001, 5);
            m_pSpinClahe = addSpin(6, tr("Clahe clip"), m_pParam->m_claheClip);
            m_pCheck = addCheck(7, tr("Use OpenCL"), m_pParam->m_bUseOCL);
        }

        void onApply() override
        {
            m_pParam->m_basisSize = cv::Size(m_pSpinSizeW->value(), m_pSpinSizeH->value());
            m_pParam->m_sparseRate = m_pSpinRate->value();
            m_pParam->m_retainedCornersFraction = m_pSpinCorners->value();
            m_pParam->m_occlusionsThreshold = m_pSpinOccTh->value();
            m_pParam->m_dampingFactor = m_pSpinDamp->value();
            m_pParam->m_claheClip = m_pSpinClahe->value();
            m_pParam->m_bUseOCL = m_pCheck->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvPCAOFParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinRate = nullptr;
        QDoubleSpinBox* m_pSpinCorners = nullptr;
        QDoubleSpinBox* m_pSpinOccTh = nullptr;
        QDoubleSpinBox* m_pSpinDamp = nullptr;
        QSpinBox*       m_pSpinSizeW = nullptr;
        QSpinBox*       m_pSpinSizeH = nullptr;
        QSpinBox*       m_pSpinClahe = nullptr;
        QCheckBox*      m_pCheck = nullptr;
};

class COcvWidgetPCAOFFactory : public CWidgetFactory
{
    public:

        COcvWidgetPCAOFFactory()
        {
            m_name = "ocv_pca_flow";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetPCAOF>(pParam);
        }
};

#endif // COCVWIDGETPCAOPTICALFLOW_HPP
