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

#ifndef CGMICWIDGETMAGICDETAILS_HPP
#define CGMICWIDGETMAGICDETAILS_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicMagicDetails.hpp"

class CGmicWidgetMagicDetails : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetMagicDetails(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetMagicDetails(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicMagicDetailsParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicMagicDetailsParam>();

            m_pSpinAmplitude = addDoubleSpin(0, tr("Amplitude"), m_pParam->m_amplitude, 0, 30);
            m_pSpinSpatialScale = addDoubleSpin(1, tr("Spatial scale"), m_pParam->m_spatialScale, 0, 10);
            m_pSpinValueScale = addDoubleSpin(2, tr("Value scale"), m_pParam->m_valueScale, 0, 20);
            m_pSpinEdges = addDoubleSpin(3, tr("Edges"), m_pParam->m_edges, -3, 3);
            m_pSpinSmoothness = addDoubleSpin(4, tr("Smoothness"), m_pParam->m_smoothness, 0, 20);

            m_pComboChannels = addCombo(5, tr("Channels"));
            for(size_t i=0; i<m_pParam->m_channels.size(); ++i)
                m_pComboChannels->addItem(QString::fromStdString(m_pParam->m_channels[i]));

            m_pComboChannels->setCurrentIndex(m_pParam->m_channel);
        }

        void onApply() override
        {
            m_pParam->m_amplitude = m_pSpinAmplitude->value();
            m_pParam->m_spatialScale = m_pSpinSpatialScale->value();
            m_pParam->m_valueScale = m_pSpinValueScale->value();
            m_pParam->m_edges = m_pSpinEdges->value();
            m_pParam->m_smoothness = m_pSpinSmoothness->value();
            m_pParam->m_channel = m_pComboChannels->currentIndex();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicMagicDetailsParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinAmplitude = nullptr;
        QDoubleSpinBox* m_pSpinSpatialScale = nullptr;
        QDoubleSpinBox* m_pSpinValueScale = nullptr;
        QDoubleSpinBox* m_pSpinEdges = nullptr;
        QDoubleSpinBox* m_pSpinSmoothness = nullptr;
        QComboBox*      m_pComboChannels = nullptr;
};

class CGmicWidgetMagicDetailsFactory : public CWidgetFactory
{
    public:

        CGmicWidgetMagicDetailsFactory()
        {
            m_name = "gmic_magic_details";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetMagicDetails>(pParam);
        }
};

#endif // CGMICWIDGETMAGICDETAILS_HPP
