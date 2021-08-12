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

#ifndef CWIDGETGRAPHICSTOBINARY_HPP
#define CWIDGETGRAPHICSTOBINARY_HPP

#include "Process/Core/CGraphicsToBinary.h"

class CWidgetGraphicsToBinary : public CWorkflowTaskWidget
{
    public:

        CWidgetGraphicsToBinary(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CWidgetGraphicsToBinary(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGraphicsToBinaryParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGraphicsToBinaryParam>();

            m_pCheckFit = addCheck(tr("Fit to content"), m_pParam->m_bFitToContent);
            m_pSpinWidth = addSpin(tr("Image width"), m_pParam->m_width);
            m_pSpinWidth->setEnabled(!m_pParam->m_bFitToContent);
            m_pSpinHeight = addSpin(tr("Image width"), m_pParam->m_width);
            m_pSpinHeight->setEnabled(!m_pParam->m_bFitToContent);

            connect(m_pCheckFit, &QCheckBox::clicked, [&](bool checked){
                m_pSpinWidth->setEnabled(!checked);
                m_pSpinHeight->setEnabled(!checked);
            });
        }

        void onApply() override
        {
            m_pParam->m_bFitToContent = m_pCheckFit->isChecked();
            m_pParam->m_width = m_pSpinWidth->value();
            m_pParam->m_height = m_pSpinHeight->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGraphicsToBinaryParam> m_pParam = nullptr;
        QCheckBox*  m_pCheckFit = nullptr;
        QSpinBox*   m_pSpinWidth = nullptr;
        QSpinBox*   m_pSpinHeight = nullptr;
};

class CWidgetGraphicsToBinaryFactory : public CWidgetFactory
{
    public:

        CWidgetGraphicsToBinaryFactory()
        {
            m_name = QObject::tr("Graphics to binary").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CWidgetGraphicsToBinary>(pParam);
        }
};

#endif // CWIDGETGRAPHICSTOBINARY_HPP
