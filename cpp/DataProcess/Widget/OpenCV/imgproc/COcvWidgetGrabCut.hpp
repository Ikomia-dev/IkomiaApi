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

#ifndef COCVWIDGETGRABCUT_HPP
#define COCVWIDGETGRABCUT_HPP

#include <QRadioButton>
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvGrabCut.hpp"

//-----------------------------//
//----- COcvWidgetGrabCut -----//
//-----------------------------//
class COcvWidgetGrabCut : public CProtocolTaskWidget
{
    public:

        COcvWidgetGrabCut(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetGrabCut(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGrabCutParam>(pParam);
            init();
        }

    protected:

        void    init() override
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGrabCutParam>();

            auto pLabelIteration = new QLabel(tr("Iteration"));
            m_pSpinIteration = new QSpinBox;
            m_pSpinIteration->setValue(m_pParam->m_iterationCount);

            m_pBckBtn = new QRadioButton(tr("Background"));
            m_pBckBtn->setChecked(true);
            m_pFgndBtn = new QRadioButton(tr("Foreground"));
            m_pPossibleBckBtn = new QRadioButton(tr("Possible background"));
            m_pPossibleFgndBtn = new QRadioButton(tr("Possible foreground"));

            auto pClassLayout = new QVBoxLayout;
            pClassLayout->addWidget(m_pBckBtn);
            pClassLayout->addWidget(m_pFgndBtn);
            pClassLayout->addWidget(m_pPossibleBckBtn);
            pClassLayout->addWidget(m_pPossibleFgndBtn);

            auto pGroupBox = new QGroupBox(tr("Current class"));
            pGroupBox->setLayout(pClassLayout);

            m_pClearBtn = new QPushButton(tr("Clear segmentation"));
            m_pClearBtn->setDefault(false);

            
            m_pLayout->addWidget(pLabelIteration, 0, 0, 1, 1);
            m_pLayout->addWidget(m_pSpinIteration, 0, 1, 1, 1);
            m_pLayout->addWidget(pGroupBox, 1, 0, 1, 2);
            m_pLayout->addWidget(m_pClearBtn, 2, 0, 1, 2);
            

            initConnections();
        }
        void    initConnections()
        {
            connect(m_pClearBtn, &QPushButton::clicked, [&]{ emit doSendProcessAction(COcvGrabCutParam::ActionFlags::CLEAR_SEGMENTATION); });

            connect(m_pBckBtn, &QRadioButton::toggled, [&](bool checked)
            {
                if(checked == true)
                {
                    emit doSendProcessAction(COcvGrabCutParam::ActionFlags::SET_BCKGND);
                    emit doSetGraphicsCategory("Background");
                }
            });
            connect(m_pFgndBtn, &QRadioButton::toggled, [&](bool checked)
            {
                if(checked == true)
                {
                    emit doSendProcessAction(COcvGrabCutParam::ActionFlags::SET_FOREGND);
                    emit doSetGraphicsCategory("Foreground");
                }
            });
            connect(m_pPossibleBckBtn, &QRadioButton::toggled, [&](bool checked)
            {
                if(checked == true)
                {
                    emit doSendProcessAction(COcvGrabCutParam::ActionFlags::SET_POSSIBLE_BCKGND);
                    emit doSetGraphicsCategory("PossibleBgnd");
                }
            });
            connect(m_pPossibleFgndBtn, &QRadioButton::toggled, [&](bool checked)
            {
                if(checked == true)
                {
                    emit doSendProcessAction(COcvGrabCutParam::ActionFlags::SET_POSSIBLE_FOREGND);
                    emit doSetGraphicsCategory("PossibleFgnd");
                }
            });

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_iterationCount = m_pSpinIteration->value();
                emit doApplyProcess(m_pParam);
            });
        }

        void    showEvent(QShowEvent* event) override
        {
            Q_UNUSED(event);

            if(m_bFirstShow == true)
            {
                emit doSetGraphicsTool(GraphicsShape::RECTANGLE);
                m_bFirstShow = false;
            }
        }

    private:

        bool                                m_bFirstShow = true;
        QSpinBox*                           m_pSpinIteration = nullptr;
        QPushButton*                        m_pClearBtn = nullptr;
        QRadioButton*                       m_pBckBtn = nullptr;
        QRadioButton*                       m_pPossibleBckBtn = nullptr;
        QRadioButton*                       m_pFgndBtn = nullptr;
        QRadioButton*                       m_pPossibleFgndBtn = nullptr;
        std::shared_ptr<COcvGrabCutParam>   m_pParam = nullptr;
};

//------------------------------------//
//----- COcvWidgetGrabCutFactory -----//
//------------------------------------//
class COcvWidgetGrabCutFactory : public CWidgetFactory
{
    public:

        COcvWidgetGrabCutFactory()
        {
            m_name = QObject::tr("GrabCut Segmentation").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGrabCut>(pParam);
        }
};

#endif // COCVWIDGETGRABCUT_HPP
