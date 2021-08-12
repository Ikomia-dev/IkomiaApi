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

#ifndef COCVWIDGETINRANGE_HPP
#define COCVWIDGETINRANGE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvInRange.hpp"

class COcvWidgetInRange : public CWorkflowTaskWidget
{
    public:

        COcvWidgetInRange(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetInRange(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvInRangeParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvInRangeParam>();

            m_pCheckLowerScalar = addCheck(0, tr("Lower scalar input"), m_pParam->m_bLowerScalar);
            m_pSpinLowerScalar = addDoubleSpin(1, tr("Lower scalar value"), m_pParam->m_lower[0]);
            m_pCheckUpperScalar = addCheck(2, tr("Upper scalar input"), m_pParam->m_bUpperScalar);
            m_pSpinUpperScalar = addDoubleSpin(3, tr("Upper scalar value"), m_pParam->m_upper[0]);
        }

        void onApply() override
        {
            m_pParam->m_bLowerScalar = m_pCheckLowerScalar->isChecked();
            m_pParam->m_lower= cv::Scalar::all(m_pSpinLowerScalar->value());
            m_pParam->m_bUpperScalar = m_pCheckUpperScalar->isChecked();
            m_pParam->m_upper= cv::Scalar::all(m_pSpinUpperScalar->value());
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvInRangeParam> m_pParam = nullptr;
        QCheckBox*      m_pCheckLowerScalar = nullptr;
        QCheckBox*      m_pCheckUpperScalar = nullptr;
        QDoubleSpinBox* m_pSpinLowerScalar = nullptr;
        QDoubleSpinBox* m_pSpinUpperScalar = nullptr;
};

class COcvWidgetInRangeFactory : public CWidgetFactory
{
    public:

        COcvWidgetInRangeFactory()
        {
            m_name = QObject::tr("InRange").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetInRange>(pParam);
        }
};

#endif // COCVWIDGETINRANGE_HPP
