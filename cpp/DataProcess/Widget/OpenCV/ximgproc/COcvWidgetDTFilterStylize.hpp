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

#ifndef COCVWIDGETDTFILTERSTYLIZE_HPP
#define COCVWIDGETDTFILTERSTYLIZE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvDTFilterStylize.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDTFilterStylize : public CWorkflowTaskWidget
{
    public:

        COcvWidgetDTFilterStylize(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDTFilterStylize(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDTFilterStylizeParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDTFilterStylizeParam>();

            m_pSpinColor = addDoubleSpin(0, tr("Sigma color"), m_pParam->m_sigmaColor, 0, DBL_MAX, 0.1);
            m_pSpinSpatial = addDoubleSpin(1, tr("Sigma spatial"), m_pParam->m_sigmaSpatial);
            m_pSpinEdges = addSpin(2, tr("Edges gamma"), m_pParam->m_edgesGamma);
        }

        void onApply() override
        {
            m_pParam->m_sigmaColor = m_pSpinColor->value();
            m_pParam->m_sigmaSpatial = m_pSpinSpatial->value();
            m_pParam->m_edgesGamma = m_pSpinEdges->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvDTFilterStylizeParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinColor = nullptr;
        QDoubleSpinBox* m_pSpinSpatial = nullptr;
        QSpinBox*       m_pSpinEdges = nullptr;
};

class COcvWidgetDTFilterStylizeFactory : public CWidgetFactory
{
    public:

        COcvWidgetDTFilterStylizeFactory()
        {
            m_name = "ocv_dt_filter_stylize";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDTFilterStylize>(pParam);
        }
};

#endif // COCVWIDGETDTFILTERSTYLIZE_HPP
