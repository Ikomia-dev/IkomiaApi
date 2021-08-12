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

#ifndef CGMICWIDGETDYNAMICRANGEINCREASE_HPP
#define CGMICWIDGETDYNAMICRANGEINCREASE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicDynamicRangeIncrease.hpp"

class CGmicWidgetDynamicRangeIncrease : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetDynamicRangeIncrease(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetDynamicRangeIncrease(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicDynamicRangeIncreaseParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicDynamicRangeIncreaseParam>();

            m_pSpinMapTones = addDoubleSpin(0, tr("Map tones"), m_pParam->m_mapTones, 0, 1);
            m_pSpinShadows = addDoubleSpin(1, tr("Recover shadows"), m_pParam->m_recoverShadows, 0, 1);
            m_pSpinHighlights = addDoubleSpin(2, tr("Recover highlights"), m_pParam->m_recoverHighligths, 0, 1);
            m_pSpinDetails = addDoubleSpin(3, tr("Enhance details"), m_pParam->m_enhanceDetails, 0, 5);
            m_pSpinDetailStrength = addDoubleSpin(4, tr("Detail strength"), m_pParam->m_detailStrength, 0, 1);
            m_pCheckMapTones = addCheck(5, tr("Map tones"), m_pParam->m_bMapTones);
            m_pCheckDetails = addCheck(6, tr("Enhance details"), m_pParam->m_bEnhanceDetails);
        }

        void onApply() override
        {
            m_pParam->m_mapTones = m_pSpinMapTones->value();
            m_pParam->m_recoverShadows = m_pSpinShadows->value();
            m_pParam->m_recoverHighligths = m_pSpinHighlights->value();
            m_pParam->m_enhanceDetails = m_pSpinDetails->value();
            m_pParam->m_detailStrength = m_pSpinDetailStrength->value();
            m_pParam->m_bMapTones = m_pCheckMapTones->isChecked();
            m_pParam->m_bEnhanceDetails = m_pCheckDetails->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicDynamicRangeIncreaseParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinMapTones = nullptr;
        QDoubleSpinBox* m_pSpinShadows = nullptr;
        QDoubleSpinBox* m_pSpinHighlights = nullptr;
        QDoubleSpinBox* m_pSpinDetails = nullptr;
        QDoubleSpinBox* m_pSpinDetailStrength = nullptr;
        QCheckBox*      m_pCheckMapTones = nullptr;
        QCheckBox*      m_pCheckDetails = nullptr;
};

class CGmicWidgetDynamicRangeIncreaseFactory : public CWidgetFactory
{
    public:

        CGmicWidgetDynamicRangeIncreaseFactory()
        {
            m_name = QObject::tr("Dynamic range increase").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetDynamicRangeIncrease>(pParam);
        }
};

#endif // CGMICWIDGETDYNAMICRANGEINCREASE_HPP
