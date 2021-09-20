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

#ifndef COCVWIDGETDTFILTERENHANCE_HPP
#define COCVWIDGETDTFILTERENHANCE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvDTFilterEnhance.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDTFilterEnhance : public CWorkflowTaskWidget
{
    public:

        COcvWidgetDTFilterEnhance(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDTFilterEnhance(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDTFilterEnhanceParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDTFilterEnhanceParam>();

            m_pSpinColor = addDoubleSpin(0, tr("Sigma color"), m_pParam->m_sigmaColor, 0, DBL_MAX, 0.1);
            m_pSpinSpatial = addDoubleSpin(1, tr("Sigma spatial"), m_pParam->m_sigmaSpatial);
            m_pSpinContrast = addSpin(2, tr("Contrast"), m_pParam->m_contrastBase);
            m_pSpinDetails = addSpin(3, tr("Sigmoid coefficient"), m_pParam->m_detailsLevel);
            m_pSpinLayers = addSpin(4, tr("Layers"), m_pParam->m_numLayer, 1);
        }

        void onApply() override
        {
            m_pParam->m_sigmaColor = m_pSpinColor->value();
            m_pParam->m_sigmaSpatial = m_pSpinSpatial->value();
            m_pParam->m_contrastBase = m_pSpinContrast->value();
            m_pParam->m_detailsLevel = m_pSpinDetails->value();
            m_pParam->m_numLayer = m_pSpinLayers->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvDTFilterEnhanceParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinColor = nullptr;
        QDoubleSpinBox* m_pSpinSpatial = nullptr;
        QSpinBox*       m_pSpinContrast = nullptr;
        QSpinBox*       m_pSpinDetails = nullptr;
        QSpinBox*       m_pSpinLayers = nullptr;
};

class COcvWidgetDTFilterEnhanceFactory : public CWidgetFactory
{
    public:

        COcvWidgetDTFilterEnhanceFactory()
        {
            m_name = "ocv_dt_filter_enhance";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDTFilterEnhance>(pParam);
        }
};

#endif // COCVWIDGETDTFILTERENHANCE_HPP
