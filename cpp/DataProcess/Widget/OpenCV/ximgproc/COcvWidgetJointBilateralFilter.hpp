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

#ifndef COCVWIDGETJOINTJointBilateralFilterFILTER_HPP
#define COCVWIDGETJOINTJointBilateralFilterFILTER_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvJointBilateralFilter.hpp"

class COcvWidgetJointBilateralFilter : public CWorkflowTaskWidget
{
    public:

        COcvWidgetJointBilateralFilter(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetJointBilateralFilter(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvJointBilateralFilterParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvJointBilateralFilterParam>();

            m_pSpinDiameter = addSpin(0, tr("Diameter"), m_pParam->m_diameter);
            m_pSpinSigmaColor = addDoubleSpin(1, tr("Sigma color"), m_pParam->m_sigmaColor);
            m_pSpinSigmaSpace = addDoubleSpin(2, tr("Sigma space"), m_pParam->m_sigmaSpace);

            m_pComboBorder = addCombo(3, tr("Border type"));
            m_pComboBorder->addItem("BORDER_DEFAULT", cv::BORDER_DEFAULT);
            m_pComboBorder->addItem("BORDER_CONSTANT", cv::BORDER_CONSTANT);
            m_pComboBorder->addItem("BORDER_REFLECT", cv::BORDER_REFLECT);
            m_pComboBorder->addItem("BORDER_WRAP", cv::BORDER_WRAP);
            m_pComboBorder->addItem("BORDER_REFLECT_101", cv::BORDER_REFLECT_101);
            m_pComboBorder->addItem("BORDER_TRANSPARENT", cv::BORDER_TRANSPARENT);
            m_pComboBorder->addItem("BORDER_ISOLATED", cv::BORDER_ISOLATED);
            m_pComboBorder->addItem("BORDER_REPLICATE", cv::BORDER_REPLICATE);
            m_pComboBorder->setCurrentIndex(m_pComboBorder->findData(m_pParam->m_borderType));
        }

        void onApply() override
        {
            m_pParam->m_diameter = m_pSpinDiameter->value();
            m_pParam->m_sigmaColor = m_pSpinSigmaColor->value();
            m_pParam->m_sigmaSpace = m_pSpinSigmaSpace->value();
            m_pParam->m_borderType = m_pComboBorder->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvJointBilateralFilterParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinSigmaColor = nullptr;
        QDoubleSpinBox* m_pSpinSigmaSpace = nullptr;
        QSpinBox*       m_pSpinDiameter = nullptr;
        QComboBox*      m_pComboBorder = nullptr;
};

class COcvWidgetJointBilateralFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetJointBilateralFilterFactory()
        {
            m_name = "ocv_joint_bilateral_filter";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetJointBilateralFilter>(pParam);
        }
};

#endif // COCVWIDGETJOINTJointBilateralFilterFILTER_HPP
