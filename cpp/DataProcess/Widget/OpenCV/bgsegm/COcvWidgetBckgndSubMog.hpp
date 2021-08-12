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

#ifndef COCVWIDGETBCKGNDSUBMOG_HPP
#define COCVWIDGETBCKGNDSUBMOG_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bgsegm/COcvBckgndSubMog.hpp"

class COcvWidgetBckgndSubMog : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBckgndSubMog(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubMog(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubMogParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubMogParam>();

            m_pSpinHistory = addSpin(0, tr("Length of history"), m_pParam->m_history);
            m_pSpinMixtures = addSpin(1, tr("Number of Gaussian mixtures"), m_pParam->m_mixturesCount);
            m_pSpinRatio = addDoubleSpin(2, tr("Background ratio"), m_pParam->m_bckRatio, 0.0, 10.0, 0.1, 1);
            m_pSpinSigma = addDoubleSpin(3, tr("Noise strength (standard deviation)"), m_pParam->m_noiseSigma, 0.0, 255.0, 1.0, 1);
        }

        void onApply() override
        {
            m_pParam->m_history = m_pSpinHistory->value();
            m_pParam->m_mixturesCount = m_pSpinMixtures->value();
            m_pParam->m_bckRatio = m_pSpinRatio->value();
            m_pParam->m_noiseSigma = m_pSpinSigma->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvBckgndSubMogParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinRatio = nullptr;
        QDoubleSpinBox* m_pSpinSigma = nullptr;
        QSpinBox*       m_pSpinHistory = nullptr;
        QSpinBox*       m_pSpinMixtures = nullptr;
};

class COcvWidgetBckgndSubMogFactory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubMogFactory()
        {
            m_name = QObject::tr("MOG background substractor").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubMog>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBMOG_HPP
