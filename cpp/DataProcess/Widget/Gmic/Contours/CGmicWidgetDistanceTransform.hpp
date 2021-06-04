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

#ifndef CGMICWIDGETDISTANCETRANSFORM_HPP
#define CGMICWIDGETDISTANCETRANSFORM_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Contours/CGmicDistanceTransform.hpp"

class CGmicWidgetDistanceTransform : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetDistanceTransform(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetDistanceTransform(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicDistanceTransformParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicDistanceTransformParam>();

            auto pSpinValue = addSpin(0, tr("Value"), m_pParam->m_value, 0, 5, 0.1);

            auto pComboMetric = addCombo(1, tr("Metric"));
            pComboMetric->addItem(tr("Chebyshev"), CGmicDistanceTransformParam::CHEBYSHEV);
            pComboMetric->addItem(tr("Manhattan"), CGmicDistanceTransformParam::MANHATTAN);
            pComboMetric->addItem(tr("Euclidean"), CGmicDistanceTransformParam::EUCLIDEAN);
            pComboMetric->addItem(tr("Square-euclidean"), CGmicDistanceTransformParam::SQUARE_EUCLIDEAN);
            pComboMetric->setCurrentIndex(pComboMetric->findData(m_pParam->m_metric));

            auto pComboNorm = addCombo(2, tr("Normalization"));
            pComboNorm->addItem(tr("Cut"), CGmicDistanceTransformParam::CUT);
            pComboNorm->addItem(tr("Normalize"), CGmicDistanceTransformParam::NORMALIZE);
            pComboNorm->addItem(tr("Modulo"), CGmicDistanceTransformParam::MODULO);
            pComboMetric->setCurrentIndex(pComboMetric->findData(m_pParam->m_normalization));

            auto pSpinModulo = addSpin(0, tr("Modulo"), m_pParam->m_modulo, 1, 255);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_value = pSpinValue->value();
                m_pParam->m_metric = pComboMetric->currentData().toInt();
                m_pParam->m_normalization = pComboNorm->currentData().toInt();
                m_pParam->m_modulo = pSpinModulo->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicDistanceTransformParam> m_pParam = nullptr;
};

class CGmicWidgetDistanceTransformFactory : public CWidgetFactory
{
    public:

        CGmicWidgetDistanceTransformFactory()
        {
            m_name = QObject::tr("Distance transform").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetDistanceTransform>(pParam);
        }
};

#endif // CGMICWIDGETDISTANCETRANSFORM_HPP
