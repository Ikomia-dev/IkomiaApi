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

#ifndef COCVWIDGETDIVIDE_HPP
#define COCVWIDGETDIVIDE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvDivide.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetDivide : public CWorkflowTaskWidget
{
    public:

        COcvWidgetDivide(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDivide(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDivideParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDivideParam>();

            auto pSpinScale = addDoubleSpin(0, tr("Scale"), m_pParam->m_scale, 0, DBL_MAX, 1, 4);
            auto pComboDepth = addCombo(1, tr("Depth"));
            pComboDepth->addItem("Default", -1);
            pComboDepth->addItem("CV_8U", CV_8U);
            pComboDepth->addItem("CV_16U", CV_16U);
            pComboDepth->addItem("CV_32F", CV_32F);
            pComboDepth->addItem("CV_64F", CV_64F);
            pComboDepth->setCurrentIndex(pComboDepth->findData(m_pParam->m_dtype));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_scale = pSpinScale->value();
                m_pParam->m_dtype = pComboDepth->currentData().toInt();
                emit doApplyProcess(m_pParam);
            } );

            
            
        }

    private:

        std::shared_ptr<COcvDivideParam> m_pParam = nullptr;
};

class COcvWidgetDivideFactory : public CWidgetFactory
{
    public:

        COcvWidgetDivideFactory()
        {
            m_name = QObject::tr("Divide").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDivide>(pParam);
        }
};
#endif // COCVWIDGETDIVIDE_HPP