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

#ifndef COCVWIDGETOBJECTNESSBING_HPP
#define COCVWIDGETOBJECTNESSBING_HPP
#include "Process/OpenCV/saliency/COcvObjectnessBING.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetObjectnessBING : public CProtocolTaskWidget
{
    public:

        COcvWidgetObjectnessBING(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetObjectnessBING(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvObjectnessBINGParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvObjectnessBINGParam>();

            auto pSpinBB = addSpin(0, tr("N best bounding boxes"), m_pParam->m_nbBBox);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_nbBBox = pSpinBB->value();
                emit doApplyProcess(m_pParam); } );

            
        }

    private:

        std::shared_ptr<COcvObjectnessBINGParam> m_pParam = nullptr;
};

class COcvWidgetObjectnessBINGFactory : public CWidgetFactory
{
    public:

        COcvWidgetObjectnessBINGFactory()
        {
            m_name = QObject::tr("ObjectnessBING").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetObjectnessBING>(pParam);
        }
};
#endif // COCVWIDGETOBJECTNESSBING_HPP
