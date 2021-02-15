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

#ifndef CGMICWIDGETBOOSTCHROMA_HPP
#define CGMICWIDGETBOOSTCHROMA_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Colors/CGmicBoostChroma.hpp"

class CGmicWidgetBoostChroma : public CProtocolTaskWidget
{
    public:

        CGmicWidgetBoostChroma(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetBoostChroma(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicBoostChromaParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicBoostChromaParam>();

            auto pSpinAmplitude = addSpin(0, tr("Amplitude(%)"), m_pParam->m_amplitude, 0, 100);
            auto pComboColorSpace = addCombo(1, tr("Color space"));
            pComboColorSpace->addItem(tr("YCbCr(distinct)"), CGmicBoostChromaParam::YCBCR);
            pComboColorSpace->addItem(tr("YCbCr(Mixed)"), CGmicBoostChromaParam::YCBCR_MIXED);
            pComboColorSpace->addItem(tr("Lab(distinct)"), CGmicBoostChromaParam::LAB);
            pComboColorSpace->addItem(tr("Lab(Mixed)"), CGmicBoostChromaParam::LAB_MIXED);




            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_amplitude = pSpinAmplitude->value();
                m_pParam->m_colorSpace = pComboColorSpace->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicBoostChromaParam> m_pParam = nullptr;
};

class CGmicWidgetBoostChromaFactory : public CWidgetFactory
{
    public:

        CGmicWidgetBoostChromaFactory()
        {
            m_name = QObject::tr("Boost chromaticity").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetBoostChroma>(pParam);
        }
};

#endif // CGMICWIDGETBOOSTCHROMA_HPP
