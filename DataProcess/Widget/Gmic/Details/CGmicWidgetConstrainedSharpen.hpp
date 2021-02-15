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

class CGmicWidgetConstrainedSharpen : public CProtocolTaskWidget
{
    public:

        CGmicWidgetConstrainedSharpen(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetConstrainedSharpen(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicConstrainedSharpenParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicConstrainedSharpenParam>();

            auto pSpinRadius = addDoubleSpin(0, tr("Sharpen radius"), m_pParam->m_radius, 0, 10);
            auto pSpinAmount = addDoubleSpin(1, tr("Amount"), m_pParam->m_amount, 0, 10);
            auto pSpinThreshold = addDoubleSpin(2, tr("Threshold"), m_pParam->m_threshold, 0, 50);
            auto pSpinConstraintRadius = addSpin(3, tr("Constraint radius"), m_pParam->m_constraintRadius, 0, 10);
            auto pSpinOvershoot = addDoubleSpin(4, tr("Overshoot"), m_pParam->m_overshoot, 0, 50);

            auto pComboChannels = addCombo(5, tr("Channels"));
            for(size_t i=0; i<m_pParam->m_channels.size(); ++i)
                pComboChannels->addItem(QString::fromStdString(m_pParam->m_channels[i]));

            pComboChannels->setCurrentIndex(m_pParam->m_channel);

            auto pComboValueAction = addCombo(6, tr("Value action"));
            pComboValueAction->addItem(tr("None"), CGmicConstrainedSharpenParam::NONE);
            pComboValueAction->addItem(tr("Cut"), CGmicConstrainedSharpenParam::CUT);
            pComboValueAction->addItem(tr("Normalize"), CGmicConstrainedSharpenParam::NORMALIZE);
            pComboValueAction->setCurrentIndex(pComboValueAction->findData(m_pParam->m_valueAction));



            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_radius = pSpinRadius->value();
                m_pParam->m_amount = pSpinAmount->value();
                m_pParam->m_threshold = pSpinThreshold->value();
                m_pParam->m_constraintRadius = pSpinConstraintRadius->value();
                m_pParam->m_overshoot = pSpinOvershoot->value();
                m_pParam->m_channel = pComboChannels->currentIndex();
                m_pParam->m_valueAction = pComboValueAction->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicConstrainedSharpenParam> m_pParam = nullptr;
};

class CGmicWidgetConstrainedSharpenFactory : public CWidgetFactory
{
    public:

        CGmicWidgetConstrainedSharpenFactory()
        {
            m_name = QObject::tr("Constrained sharpen").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetConstrainedSharpen>(pParam);
        }
};

#endif // CGMICWIDGETCONSTRAINEDSHARPEN_HPP
