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

#ifndef COCVWIDGETCOPYMAKEBORDER_HPP
#define COCVWIDGETCOPYMAKEBORDER_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvCopyMakeBorder.hpp"

class COcvWidgetCopyMakeBorder : public CWorkflowTaskWidget
{
    public:

        COcvWidgetCopyMakeBorder(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCopyMakeBorder(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCopyMakeBorderParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCopyMakeBorderParam>();

            m_pSpinTop = addSpin(0, tr("Top"), m_pParam->m_top, 0, 10000, 1);
            m_pSpinBottom = addSpin(1, tr("Bottom"), m_pParam->m_bottom, 0, 10000, 1);
            m_pSpinLeft = addSpin(2, tr("Left"), m_pParam->m_left, 0, 10000, 1);
            m_pSpinRight = addSpin(3, tr("Right"), m_pParam->m_right, 0, 10000, 1);

            m_pComboBorder = addCombo(4, tr("Border type"));
            m_pComboBorder->addItem("BORDER_DEFAULT", cv::BORDER_DEFAULT);
            //m_pComboBorder->addItem("BORDER_CONSTANT", cv::BORDER_CONSTANT);
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
            m_pParam->m_top = m_pSpinTop->value();
            m_pParam->m_bottom = m_pSpinBottom->value();
            m_pParam->m_left = m_pSpinLeft->value();
            m_pParam->m_right = m_pSpinRight->value();
            m_pParam->m_borderType = m_pComboBorder->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvCopyMakeBorderParam>  m_pParam = nullptr;
        QSpinBox*   m_pSpinTop = nullptr;
        QSpinBox*   m_pSpinBottom = nullptr;
        QSpinBox*   m_pSpinLeft = nullptr;
        QSpinBox*   m_pSpinRight = nullptr;
        QComboBox*  m_pComboBorder = nullptr;
};

class COcvWidgetCopyMakeBorderFactory : public CWidgetFactory
{
    public:

        COcvWidgetCopyMakeBorderFactory()
        {
            m_name = "ocv_copy_make_border";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetCopyMakeBorder>(pParam);
        }
};

#endif // COCVWIDGETCOPYMAKEBORDER_HPP
