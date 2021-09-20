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

#ifndef COCVWIDGETMEDIAN_HPP
#define COCVWIDGETMEDIAN_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvMedian.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetMedian : public CWorkflowTaskWidget
{
    public:

        COcvWidgetMedian(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetMedian(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMedianParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvMedianParam>();

            m_pSpinSize = addSpin(0, tr("Filter size"), m_pParam->m_ksize, 1, INT_MAX, 2);
        }

        void onApply() override
        {
            m_pParam->m_ksize = m_pSpinSize->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvMedianParam> m_pParam = nullptr;
        QSpinBox*   m_pSpinSize = nullptr;
};

class COcvWidgetMedianFactory : public CWidgetFactory
{
    public:

        COcvWidgetMedianFactory()
        {
            m_name = "ocv_median_filter";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetMedian>(pParam);
        }
};

#endif // COCVWIDGETMEDIAN_HPP
