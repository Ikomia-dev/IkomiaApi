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

#ifndef CGMICWIDGETSHARPENLUCY_HPP
#define CGMICWIDGETSHARPENLUCY_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicSharpenLucy.hpp"

class CGmicWidgetSharpenLucy : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetSharpenLucy(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSharpenLucy(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSharpenLucyParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSharpenLucyParam>();

            m_pSpinSigma = addDoubleSpin(0, tr("Sigma"), m_pParam->m_sigma, 0.5, 10, 0.1);
            m_pSpinIteration = addSpin(1, tr("Iterations"), m_pParam->m_iteration, 1, 100);

            m_pComboBlur = addCombo(2, tr("Blur"));
            m_pComboBlur->addItem(tr("Exponential"), CGmicSharpenLucyParam::EXPONENTIAL);
            m_pComboBlur->addItem(tr("Gaussian"), CGmicSharpenLucyParam::GAUSSIAN);
            m_pComboBlur->setCurrentIndex(m_pComboBlur->findData(m_pParam->m_blur));

            m_pCheckCut = addCheck(3, tr("Cut"), m_pParam->m_bCut);
        }

        void onApply() override
        {
            m_pParam->m_sigma = m_pSpinSigma->value();
            m_pParam->m_iteration = m_pSpinIteration->value();
            m_pParam->m_blur = m_pComboBlur->currentData().toInt();
            m_pParam->m_bCut = m_pCheckCut->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicSharpenLucyParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinSigma = nullptr;
        QSpinBox*       m_pSpinIteration = nullptr;
        QComboBox*      m_pComboBlur = nullptr;
        QCheckBox*      m_pCheckCut = nullptr;
};

class CGmicWidgetSharpenLucyFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSharpenLucyFactory()
        {
            m_name = QObject::tr("Sharpen [Richardson-Lucy]").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSharpenLucy>(pParam);
        }
};

#endif // CGMICWIDGETSHARPENLUCY_HPP
