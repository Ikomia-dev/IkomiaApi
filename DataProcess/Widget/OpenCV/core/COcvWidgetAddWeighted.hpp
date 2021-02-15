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

#ifndef COCVWIDGETADDWEIGHTED_HPP
#define COCVWIDGETADDWEIGHTED_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvAddWeighted.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetAddWeighted : public CProtocolTaskWidget
{
    public:

        COcvWidgetAddWeighted(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetAddWeighted(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvAddWeightedParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvAddWeightedParam>();

            auto pLabelAlpha = new QLabel(tr("Alpha"));
            auto pSpinAlpha = new QDoubleSpinBox;
            auto pLabelBeta = new QLabel(tr("Beta"));
            auto pSpinBeta = new QDoubleSpinBox;
            auto pLabelGamma = new QLabel(tr("Gamma"));
            auto pSpinGamma = new QDoubleSpinBox;

            pSpinAlpha->setSingleStep(0.1);
            pSpinBeta->setSingleStep(0.1);
            pSpinGamma->setSingleStep(0.1);

            pSpinAlpha->setValue(m_pParam->m_alpha);
            pSpinBeta->setValue(m_pParam->m_beta);
            pSpinGamma->setValue(m_pParam->m_gamma);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_alpha = pSpinAlpha->value();
                m_pParam->m_beta = pSpinBeta->value();
                m_pParam->m_gamma = pSpinGamma->value();
                emit doApplyProcess(m_pParam);
            } );

            
            m_pLayout->addWidget(pLabelAlpha, 0, 0);
            m_pLayout->addWidget(pSpinAlpha, 0, 1);

            m_pLayout->addWidget(pLabelBeta, 1, 0);
            m_pLayout->addWidget(pSpinBeta, 1, 1);

            m_pLayout->addWidget(pLabelGamma, 2, 0);
            m_pLayout->addWidget(pSpinGamma, 2, 1);

            
        }

    private:

        std::shared_ptr<COcvAddWeightedParam> m_pParam = nullptr;
};

class COcvWidgetAddWeightedFactory : public CWidgetFactory
{
    public:

        COcvWidgetAddWeightedFactory()
        {
            m_name = QObject::tr("Add Weighted").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetAddWeighted>(pParam);
        }
};

#endif // COCVWIDGETADDWEIGHTED_HPP
