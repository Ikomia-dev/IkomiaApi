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

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSubtractParam>();

            auto pCheck1 = addCheck(0, tr("Image 1"), !m_pParam->m_bUseValue1);
            auto pSpin1 = addDoubleSpin(1, tr("Value 1"), m_pParam->m_value1);
            auto pCheck2 = addCheck(2, tr("Image 2"), !m_pParam->m_bUseValue2);
            auto pSpin2 = addDoubleSpin(3, tr("Value 2"), m_pParam->m_value2);

            pSpin1->setEnabled(false);
            pSpin2->setEnabled(false);

            connect(pCheck1, &QCheckBox::clicked, [=](bool checked){ pSpin1->setEnabled(!checked); });
            connect(pCheck2, &QCheckBox::clicked, [=](bool checked){ pSpin2->setEnabled(!checked); });

            auto pComboDepth = addCombo(4, tr("Depth"));
            pComboDepth->addItem("Default", -1);
            pComboDepth->addItem("CV_8U", CV_8U);
            pComboDepth->addItem("CV_16U", CV_16U);
            pComboDepth->addItem("CV_32F", CV_32F);
            pComboDepth->addItem("CV_64F", CV_64F);
            pComboDepth->setCurrentIndex(pComboDepth->findData(m_pParam->m_dtype));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_dtype = pComboDepth->currentData().toInt();
                m_pParam->m_bUseValue1 = !pCheck1->isChecked();
                m_pParam->m_bUseValue2 = !pCheck2->isChecked();
                m_pParam->m_value1 = pSpin1->value();
                m_pParam->m_value2 = pSpin2->value();
                emit doApplyProcess(m_pParam);
            } );

            
        }

    private:

        std::shared_ptr<COcvSubtractParam> m_pParam = nullptr;
};

class COcvWidgetSubtractFactory : public CWidgetFactory
{
    public:

        COcvWidgetSubtractFactory()
        {
            m_name = QObject::tr("Subtract").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSubtract>(pParam);
        }
};
#endif // COCVWIDGETSUBTRACT_HPP
