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

class COcvWidgetAnisotropicDiffusion : public CWorkflowTaskWidget
{
    public:

        COcvWidgetAnisotropicDiffusion(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetAnisotropicDiffusion(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvAnisotropicDiffusionParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvAnisotropicDiffusionParam>();

            auto pLabelAlpha = new QLabel(QObject::tr("Alpha [0 - 1]"));
            m_pSpinAlpha = new QDoubleSpinBox;
            m_pSpinAlpha->setValue(m_pParam->m_alpha);
            m_pSpinAlpha->setRange(0.1, 1.0);
            m_pSpinAlpha->setSingleStep(0.1);

            auto pLabelK = new QLabel(QObject::tr("Sensitivity to edges K"));
            m_pSpinK = new QDoubleSpinBox;
            m_pSpinK->setValue(m_pParam->m_k);
            m_pSpinK->setRange(0.1, 100.0);
            m_pSpinK->setSingleStep(0.1);

            auto pLabelIteration = new QLabel(QObject::tr("Number of iteration"));
            m_pSpinIteration = new QSpinBox;
            m_pSpinIteration->setRange(1, 100);
            m_pSpinIteration->setValue(m_pParam->m_nbIter);
            m_pSpinIteration->setSingleStep(1);
            
            m_pLayout->addWidget(pLabelAlpha, 0, 0);
            m_pLayout->addWidget(m_pSpinAlpha, 0, 1);
            m_pLayout->addWidget(pLabelK, 1, 0);
            m_pLayout->addWidget(m_pSpinK, 1, 1);
            m_pLayout->addWidget(pLabelIteration, 2, 0);
            m_pLayout->addWidget(m_pSpinIteration, 2, 1);
        }

        void onApply() override
        {
            m_pParam->m_alpha = m_pSpinAlpha->value();
            m_pParam->m_k = m_pSpinK->value();
            m_pParam->m_nbIter = m_pSpinIteration->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvAnisotropicDiffusionParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinAlpha = nullptr;
        QDoubleSpinBox* m_pSpinK = nullptr;
        QSpinBox*       m_pSpinIteration = nullptr;
};

class COcvWidgetAnisotropicDiffusionFactory : public CWidgetFactory
{
    public:

        COcvWidgetAnisotropicDiffusionFactory()
        {
            m_name = "ocv_anisotropic_diffusion";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetAnisotropicDiffusion>(pParam);
        }
};

#endif // COCVWIDGETANISOTROPICDIFFUSION_HPP
