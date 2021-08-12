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

#ifndef CGMICWIDGETCONSTRAINEDSHARPEN_HPP
#define CGMICWIDGETCONSTRAINEDSHARPEN_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicConstrainedSharpen.hpp"

class CGmicWidgetConstrainedSharpen : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetConstrainedSharpen(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetConstrainedSharpen(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicConstrainedSharpenParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicConstrainedSharpenParam>();

            m_pSpinRadius = addDoubleSpin(0, tr("Sharpen radius"), m_pParam->m_radius, 0, 10);
            m_pSpinAmount = addDoubleSpin(1, tr("Amount"), m_pParam->m_amount, 0, 10);
            m_pSpinThreshold = addDoubleSpin(2, tr("Threshold"), m_pParam->m_threshold, 0, 50);
            m_pSpinConstraintRadius = addSpin(3, tr("Constraint radius"), m_pParam->m_constraintRadius, 0, 10);
            m_pSpinOvershoot = addDoubleSpin(4, tr("Overshoot"), m_pParam->m_overshoot, 0, 50);

            m_pComboChannels = addCombo(5, tr("Channels"));
            for(size_t i=0; i<m_pParam->m_channels.size(); ++i)
                m_pComboChannels->addItem(QString::fromStdString(m_pParam->m_channels[i]));

            m_pComboChannels->setCurrentIndex(m_pParam->m_channel);

            m_pComboValueAction = addCombo(6, tr("Value action"));
            m_pComboValueAction->addItem(tr("None"), CGmicConstrainedSharpenParam::NONE);
            m_pComboValueAction->addItem(tr("Cut"), CGmicConstrainedSharpenParam::CUT);
            m_pComboValueAction->addItem(tr("Normalize"), CGmicConstrainedSharpenParam::NORMALIZE);
            m_pComboValueAction->setCurrentIndex(m_pComboValueAction->findData(m_pParam->m_valueAction));
        }

        void onApply() override
        {
            m_pParam->m_radius = m_pSpinRadius->value();
            m_pParam->m_amount = m_pSpinAmount->value();
            m_pParam->m_threshold = m_pSpinThreshold->value();
            m_pParam->m_constraintRadius = m_pSpinConstraintRadius->value();
            m_pParam->m_overshoot = m_pSpinOvershoot->value();
            m_pParam->m_channel = m_pComboChannels->currentIndex();
            m_pParam->m_valueAction = m_pComboValueAction->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicConstrainedSharpenParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinRadius = nullptr;
        QDoubleSpinBox* m_pSpinAmount = nullptr;
        QDoubleSpinBox* m_pSpinThreshold = nullptr;
        QDoubleSpinBox* m_pSpinOvershoot = nullptr;
        QSpinBox*       m_pSpinConstraintRadius = nullptr;
        QComboBox*      m_pComboChannels = nullptr;
        QComboBox*      m_pComboValueAction = nullptr;
};

class CGmicWidgetConstrainedSharpenFactory : public CWidgetFactory
{
    public:

        CGmicWidgetConstrainedSharpenFactory()
        {
            m_name = QObject::tr("Constrained sharpen").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetConstrainedSharpen>(pParam);
        }
};

#endif // CGMICWIDGETCONSTRAINEDSHARPEN_HPP
