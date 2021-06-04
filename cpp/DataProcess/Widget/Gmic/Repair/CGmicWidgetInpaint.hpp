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

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicInpaintParam>();

            auto pSpinPatchSize = addSpin(0, tr("Patch size"), m_pParam->m_patchSize);
            auto pSpinLookupSize = addSpin(1, tr("Lookup size"), m_pParam->m_lookupSize);
            auto pSpinLookupFactor = addDoubleSpin(2, tr("Lookup factor"), m_pParam->m_lookupFactor);
            auto pSpinLookupInc = addSpin(3, tr("Lookup increment"), m_pParam->m_lookupInc);
            auto pSpinBlendSize = addSpin(4, tr("Blend size"), m_pParam->m_blendSize);
            auto pSpinBlendThresh = addSpin(5, tr("Blend threshold"), m_pParam->m_blendThreshold);
            auto pSpinBlendDecay = addDoubleSpin(6, tr("Blend decay"), m_pParam->m_blendDecay);
            auto pSpinBlendScales = addSpin(7, tr("Blend scales"), m_pParam->m_blendScales);
            auto pCheckBlendOuter = addCheck(8, tr("Outer blending"), m_pParam->m_bBlendOuter);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_patchSize = pSpinPatchSize->value();
                m_pParam->m_lookupSize = pSpinLookupSize->value();
                m_pParam->m_lookupFactor = pSpinLookupFactor->value();
                m_pParam->m_lookupInc = pSpinLookupInc->value();
                m_pParam->m_blendSize = pSpinBlendSize->value();
                m_pParam->m_blendThreshold = pSpinBlendThresh->value();
                m_pParam->m_blendDecay = pSpinBlendDecay->value();
                m_pParam->m_blendScales = pSpinBlendScales->value();
                m_pParam->m_bBlendOuter = pCheckBlendOuter->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicInpaintParam> m_pParam = nullptr;
};

class CGmicWidgetInpaintFactory : public CWidgetFactory
{
    public:

        CGmicWidgetInpaintFactory()
        {
            m_name = QObject::tr("Patch-based Inpaint").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetInpaint>(pParam);
        }
};

#endif // CGMICWIDGETINPAINT_HPP
