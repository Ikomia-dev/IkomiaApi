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

#ifndef CGMICWIDGETINPAINT_HPP
#define CGMICWIDGETINPAINT_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Repair/CGmicInpaint.hpp"

class CGmicWidgetInpaint : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetInpaint(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetInpaint(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicInpaintParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicInpaintParam>();

            m_pSpinPatchSize = addSpin(0, tr("Patch size"), m_pParam->m_patchSize);
            m_pSpinLookupSize = addSpin(1, tr("Lookup size"), m_pParam->m_lookupSize);
            m_pSpinLookupFactor = addDoubleSpin(2, tr("Lookup factor"), m_pParam->m_lookupFactor);
            m_pSpinLookupInc = addSpin(3, tr("Lookup increment"), m_pParam->m_lookupInc);
            m_pSpinBlendSize = addSpin(4, tr("Blend size"), m_pParam->m_blendSize);
            m_pSpinBlendThresh = addSpin(5, tr("Blend threshold"), m_pParam->m_blendThreshold);
            m_pSpinBlendDecay = addDoubleSpin(6, tr("Blend decay"), m_pParam->m_blendDecay);
            m_pSpinBlendScales = addSpin(7, tr("Blend scales"), m_pParam->m_blendScales);
            m_pCheckBlendOuter = addCheck(8, tr("Outer blending"), m_pParam->m_bBlendOuter);
        }

        void onApply() override
        {
            m_pParam->m_patchSize = m_pSpinPatchSize->value();
            m_pParam->m_lookupSize = m_pSpinLookupSize->value();
            m_pParam->m_lookupFactor = m_pSpinLookupFactor->value();
            m_pParam->m_lookupInc = m_pSpinLookupInc->value();
            m_pParam->m_blendSize = m_pSpinBlendSize->value();
            m_pParam->m_blendThreshold = m_pSpinBlendThresh->value();
            m_pParam->m_blendDecay = m_pSpinBlendDecay->value();
            m_pParam->m_blendScales = m_pSpinBlendScales->value();
            m_pParam->m_bBlendOuter = m_pCheckBlendOuter->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicInpaintParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinLookupFactor = nullptr;
        QDoubleSpinBox* m_pSpinBlendDecay = nullptr;
        QSpinBox*       m_pSpinPatchSize = nullptr;
        QSpinBox*       m_pSpinLookupSize = nullptr;
        QSpinBox*       m_pSpinLookupInc = nullptr;
        QSpinBox*       m_pSpinBlendSize = nullptr;
        QSpinBox*       m_pSpinBlendThresh = nullptr;
        QSpinBox*       m_pSpinBlendScales = nullptr;
        QCheckBox*      m_pCheckBlendOuter = nullptr;
};

class CGmicWidgetInpaintFactory : public CWidgetFactory
{
    public:

        CGmicWidgetInpaintFactory()
        {
            m_name = "gmic_patch_based_inpaint";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetInpaint>(pParam);
        }
};

#endif // CGMICWIDGETINPAINT_HPP
