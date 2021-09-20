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

#ifndef CGMICWIDGETDEBLUR_HPP
#define CGMICWIDGETDEBLUR_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicSharpenDeblur.hpp"

class CGmicWidgetSharpenDeblur : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetSharpenDeblur(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSharpenDeblur(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSharpenDeblurParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSharpenDeblurParam>();

            m_pSpinRadius = addDoubleSpin(0, tr("Radius"), m_pParam->m_radius, 0, 20);
            m_pSpinIteration = addSpin(1, tr("Iterations"), m_pParam->m_iteration, 0, 100);
            m_pSpinTimeStep = addDoubleSpin(2, tr("Time step"), m_pParam->m_timeStep, 0, 50);
            m_pSpinSmoothness = addDoubleSpin(3, tr("Smoothness"), m_pParam->m_smoothness, 0, 10);
            m_pSpinOverlap = addSpin(1, tr("Iterations"), m_pParam->m_spatialOverlap, 0, 256);

            m_pComboRegularization = addCombo(4, tr("Regularization"));
            m_pComboRegularization->addItem(tr("Tikhonov"), CGmicSharpenDeblurParam::TIKHONOV);
            m_pComboRegularization->addItem(tr("Mean curvature"), CGmicSharpenDeblurParam::MEAN_CURVATURE);
            m_pComboRegularization->addItem("Total variation", CGmicSharpenDeblurParam::TV);
            m_pComboRegularization->setCurrentIndex(m_pComboRegularization->findData(m_pParam->m_regularization));

            m_pComboChannels = addCombo(5, tr("Channels"));
            for(size_t i=0; i<m_pParam->m_channels.size(); ++i)
                m_pComboChannels->addItem(QString::fromStdString(m_pParam->m_channels[i]));

            m_pComboChannels->setCurrentIndex(m_pParam->m_channel);
        }

        void onApply() override
        {
            m_pParam->m_radius = m_pSpinRadius->value();
            m_pParam->m_iteration = m_pSpinIteration->value();
            m_pParam->m_timeStep = m_pSpinTimeStep->value();
            m_pParam->m_smoothness = m_pSpinSmoothness->value();
            m_pParam->m_spatialOverlap = m_pSpinOverlap->value();
            m_pParam->m_regularization = m_pComboRegularization->currentData().toInt();
            m_pParam->m_channel = m_pComboChannels->currentIndex();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicSharpenDeblurParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinRadius = nullptr;
        QDoubleSpinBox* m_pSpinTimeStep = nullptr;
        QDoubleSpinBox* m_pSpinSmoothness = nullptr;
        QSpinBox*       m_pSpinIteration = nullptr;
        QSpinBox*       m_pSpinOverlap = nullptr;
        QComboBox*      m_pComboRegularization = nullptr;
        QComboBox*      m_pComboChannels = nullptr;
};

class CGmicWidgetSharpenDeblurFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSharpenDeblurFactory()
        {
            m_name = "gmic_sharpen_deblur";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSharpenDeblur>(pParam);
        }
};

#endif // CGMICWIDGETDEBLUR_HPP
