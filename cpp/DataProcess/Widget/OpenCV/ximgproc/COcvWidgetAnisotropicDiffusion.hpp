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

#ifndef COCVWIDGETANISOTROPICDIFFUSION_HPP
#define COCVWIDGETANISOTROPICDIFFUSION_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvAnisotropicDiffusion.hpp"

class COcvWidgetAnisotropicDiffusion : public CProtocolTaskWidget
{
    public:

        COcvWidgetAnisotropicDiffusion(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetAnisotropicDiffusion(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvAnisotropicDiffusionParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvAnisotropicDiffusionParam>();

            auto pLabelAlpha = new QLabel(QObject::tr("Alpha [0 - 1]"));
            auto pSpinAlpha = new QDoubleSpinBox;
            pSpinAlpha->setValue(m_pParam->m_alpha);
            pSpinAlpha->setRange(0.1, 1.0);
            pSpinAlpha->setSingleStep(0.1);

            auto pLabelK = new QLabel(QObject::tr("Sensitivity to edges K"));
            auto pSpinK = new QDoubleSpinBox;
            pSpinK->setValue(m_pParam->m_k);
            pSpinK->setRange(0.1, 100.0);
            pSpinK->setSingleStep(0.1);

            auto pLabelIteration = new QLabel(QObject::tr("Number of iteration"));
            auto pSpinIteration = new QSpinBox;
            pSpinIteration->setRange(1, 100);
            pSpinIteration->setValue(m_pParam->m_nbIter);
            pSpinIteration->setSingleStep(1);

            
            m_pLayout->addWidget(pLabelAlpha, 0, 0);
            m_pLayout->addWidget(pSpinAlpha, 0, 1);
            m_pLayout->addWidget(pLabelK, 1, 0);
            m_pLayout->addWidget(pSpinK, 1, 1);
            m_pLayout->addWidget(pLabelIteration, 2, 0);
            m_pLayout->addWidget(pSpinIteration, 2, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_alpha = pSpinAlpha->value();
                m_pParam->m_k = pSpinK->value();
                m_pParam->m_nbIter = pSpinIteration->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvAnisotropicDiffusionParam> m_pParam = nullptr;
};

class COcvWidgetAnisotropicDiffusionFactory : public CWidgetFactory
{
    public:

        COcvWidgetAnisotropicDiffusionFactory()
        {
            m_name = QObject::tr("Anisotropic Diffusion").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetAnisotropicDiffusion>(pParam);
        }
};

#endif // COCVWIDGETANISOTROPICDIFFUSION_HPP
