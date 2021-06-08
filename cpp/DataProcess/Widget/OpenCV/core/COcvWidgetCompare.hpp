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

#ifndef COCVWIDGETCOMPARE_HPP
#define COCVWIDGETCOMPARE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvCompare.hpp"

class COcvWidgetCompare : public CWorkflowTaskWidget
{
    public:

        COcvWidgetCompare(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCompare(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCompareParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCompareParam>();

            auto pComboOp = addCombo(1, tr("Operation"));
            pComboOp->addItem(tr("Equal"), cv::CMP_EQ);
            pComboOp->addItem(tr("Greater"), cv::CMP_GT);
            pComboOp->addItem(tr("Greater or equal"), cv::CMP_GE);
            pComboOp->addItem(tr("Less"), cv::CMP_LT);
            pComboOp->addItem(tr("Less or equal"), cv::CMP_LE);
            pComboOp->addItem(tr("Unequal"), cv::CMP_NE);
            pComboOp->setCurrentIndex(pComboOp->findData(m_pParam->m_operation));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_operation = pComboOp->currentData().toInt();
                emit doApplyProcess(m_pParam);
            } );

            
            
        }

    private:

        std::shared_ptr<COcvCompareParam> m_pParam = nullptr;
};

class COcvWidgetCompareFactory : public CWidgetFactory
{
    public:

        COcvWidgetCompareFactory()
        {
            m_name = QObject::tr("Compare").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetCompare>(pParam);
        }
};

#endif // COCVWIDGETCOMPARE_HPP
