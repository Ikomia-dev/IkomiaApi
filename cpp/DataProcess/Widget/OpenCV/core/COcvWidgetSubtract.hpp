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

#ifndef COCVWIDGETSUBTRACT_HPP
#define COCVWIDGETSUBTRACT_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvSubtract.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetSubtract : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSubtract(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSubtract(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSubtractParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSubtractParam>();

            m_pCheck1 = addCheck(0, tr("Image 1"), !m_pParam->m_bUseValue1);
            m_pSpin1 = addDoubleSpin(1, tr("Value 1"), m_pParam->m_value1);
            m_pCheck2 = addCheck(2, tr("Image 2"), !m_pParam->m_bUseValue2);
            m_pSpin2 = addDoubleSpin(3, tr("Value 2"), m_pParam->m_value2);

            m_pSpin1->setEnabled(false);
            m_pSpin2->setEnabled(false);

            connect(m_pCheck1, &QCheckBox::clicked, [=](bool checked){ m_pSpin1->setEnabled(!checked); });
            connect(m_pCheck2, &QCheckBox::clicked, [=](bool checked){ m_pSpin2->setEnabled(!checked); });

            m_pComboDepth = addCombo(4, tr("Depth"));
            m_pComboDepth->addItem("Default", -1);
            m_pComboDepth->addItem("CV_8U", CV_8U);
            m_pComboDepth->addItem("CV_16U", CV_16U);
            m_pComboDepth->addItem("CV_32F", CV_32F);
            m_pComboDepth->addItem("CV_64F", CV_64F);
            m_pComboDepth->setCurrentIndex(m_pComboDepth->findData(m_pParam->m_dtype));
        }

        void onApply() override
        {
            m_pParam->m_dtype = m_pComboDepth->currentData().toInt();
            m_pParam->m_bUseValue1 = !m_pCheck1->isChecked();
            m_pParam->m_bUseValue2 = !m_pCheck2->isChecked();
            m_pParam->m_value1 = m_pSpin1->value();
            m_pParam->m_value2 = m_pSpin2->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSubtractParam> m_pParam = nullptr;
        QComboBox*  m_pComboDepth = nullptr;
        QCheckBox*  m_pCheck1 = nullptr;
        QCheckBox*  m_pCheck2 = nullptr;
        QDoubleSpinBox* m_pSpin1 = nullptr;
        QDoubleSpinBox* m_pSpin2 = nullptr;
};

class COcvWidgetSubtractFactory : public CWidgetFactory
{
    public:

        COcvWidgetSubtractFactory()
        {
            m_name = "ocv_subtract";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSubtract>(pParam);
        }
};
#endif // COCVWIDGETSUBTRACT_HPP
