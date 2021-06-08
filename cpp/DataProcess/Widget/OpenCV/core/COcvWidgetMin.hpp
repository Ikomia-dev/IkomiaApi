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

#ifndef COCVWIDGETMIN_HPP
#define COCVWIDGETMIN_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvMin.hpp"

class COcvWidgetMin : public CWorkflowTaskWidget
{
    public:

        COcvWidgetMin(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetMin(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMinParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvMinParam>();

            auto pCheckScalar = addCheck(0, tr("Scalar operation"), m_pParam->m_bScalar);
            auto pSpinScalar = addDoubleSpin(1, tr("Scalar value"), m_pParam->m_scalar[0]);
            
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_bScalar = pCheckScalar->isChecked();
                m_pParam->m_scalar= cv::Scalar::all(pSpinScalar->value());
                emit doApplyProcess(m_pParam);
            } );
        }

    private:

        std::shared_ptr<COcvMinParam> m_pParam = nullptr;
};

class COcvWidgetMinFactory : public CWidgetFactory
{
    public:

        COcvWidgetMinFactory()
        {
            m_name = QObject::tr("Min").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetMin>(pParam);
        }
};

#endif // COCVWIDGETMIN_HPP
