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

#ifndef COCVWIDGETPENCILSKETCH_HPP
#define COCVWIDGETPENCILSKETCH_HPP
#include "../../../Process/OpenCV/photo/COcvPencilSketch.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetPencilSketch : public CProtocolTaskWidget
{
    public:

        COcvWidgetPencilSketch(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetPencilSketch(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvPencilSketchParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvPencilSketchParam>();

            auto pLabelSigmaS = new QLabel(tr("Sigma spatial"));
            auto pLabelSigmaR = new QLabel(tr("Sigma range"));
            auto pLabelShade = new QLabel(tr("Shade factor"));

            auto pSpinSigmaS = new QDoubleSpinBox;
            auto pSpinSigmaR = new QDoubleSpinBox;
            auto pSpinShade = new QDoubleSpinBox;

            pSpinSigmaS->setValue(m_pParam->m_sigmaS);
            pSpinSigmaS->setRange(0, 100);
            pSpinSigmaS->setSingleStep(1);

            pSpinSigmaR->setValue(m_pParam->m_sigmaR);
            pSpinSigmaR->setRange(0, 1);
            pSpinSigmaR->setSingleStep(0.01);

            pSpinShade->setValue(m_pParam->m_shadeFactor);
            pSpinShade->setRange(0, 1);
            pSpinShade->setSingleStep(0.01);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_sigmaS = pSpinSigmaS->value();
                m_pParam->m_sigmaR = pSpinSigmaR->value();
                m_pParam->m_shadeFactor = pSpinShade->value();
                emit doApplyProcess(m_pParam); } );

            
            m_pLayout->addWidget(pLabelSigmaS, 0, 0);
            m_pLayout->addWidget(pSpinSigmaS, 0, 1);

            m_pLayout->addWidget(pLabelSigmaR, 1, 0);
            m_pLayout->addWidget(pSpinSigmaR, 1, 1);

            m_pLayout->addWidget(pLabelShade, 2, 0);
            m_pLayout->addWidget(pSpinShade, 2, 1);

            
        }

    private:

        std::shared_ptr<COcvPencilSketchParam> m_pParam = nullptr;
};

class COcvWidgetPencilSketchFactory : public CWidgetFactory
{
    public:

        COcvWidgetPencilSketchFactory()
        {
            m_name = QObject::tr("Pencil sketch").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetPencilSketch>(pParam);
        }
};

#endif // COCVWIDGETPENCILSKETCH_HPP
