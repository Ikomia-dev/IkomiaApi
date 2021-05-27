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

class CWidgetGraphicsToBinary : public CProtocolTaskWidget
{
    public:

        CWidgetGraphicsToBinary(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CWidgetGraphicsToBinary(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGraphicsToBinaryParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGraphicsToBinaryParam>();

            auto pCheckFit = addCheck(tr("Fit to content"), m_pParam->m_bFitToContent);
            auto pSpinWidth = addSpin(tr("Image width"), m_pParam->m_width);
            auto pSpinHeight = addSpin(tr("Image width"), m_pParam->m_width);
            pSpinWidth->setEnabled(!m_pParam->m_bFitToContent);
            pSpinHeight->setEnabled(!m_pParam->m_bFitToContent);

            connect(pCheckFit, &QCheckBox::clicked, [=](bool checked){
                pSpinWidth->setEnabled(!checked);
                pSpinHeight->setEnabled(!checked);
            });

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
               m_pParam->m_bFitToContent = pCheckFit->isChecked();
               m_pParam->m_width = pSpinWidth->value();
               m_pParam->m_height = pSpinHeight->value();
               emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGraphicsToBinaryParam> m_pParam = nullptr;
};

class CWidgetGraphicsToBinaryFactory : public CWidgetFactory
{
    public:

        CWidgetGraphicsToBinaryFactory()
        {
            m_name = QObject::tr("Graphics to binary").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CWidgetGraphicsToBinary>(pParam);
        }
};

#endif // CWIDGETGRAPHICSTOBINARY_HPP
