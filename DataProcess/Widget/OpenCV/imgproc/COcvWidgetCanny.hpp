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

#ifndef COCVWIDGETCANNY_HPP
#define COCVWIDGETCANNY_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvCanny.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetCanny : public CProtocolTaskWidget
{
    public:

        COcvWidgetCanny(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCanny(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCannyParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCannyParam>();

            auto pSpinTh1 = addDoubleSpin(0, tr("Threshold 1"), m_pParam->m_threshold1);
            auto pSpinTh2 = addDoubleSpin(1, tr("Threshold 2"), m_pParam->m_threshold2);
            auto pComboAperture = addCombo(2, tr("Aperture size"));
            pComboAperture->addItem("3", 3);
            pComboAperture->addItem("5", 5);
            pComboAperture->addItem("7", 7);
            pComboAperture->setCurrentIndex(pComboAperture->findData(m_pParam->m_apertureSize));

            auto pCheck = addCheck(3, tr("Use L2 gradient"), m_pParam->m_L2gradient);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_threshold1 = pSpinTh1->value();
                m_pParam->m_threshold2 = pSpinTh2->value();
                m_pParam->m_apertureSize = pComboAperture->currentData().toInt();
                m_pParam->m_L2gradient = pCheck->isChecked();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvCannyParam> m_pParam = nullptr;
};

class COcvWidgetCannyFactory : public CWidgetFactory
{
    public:

        COcvWidgetCannyFactory()
        {
            m_name = QObject::tr("Canny").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetCanny>(pParam);
        }
};

#endif // COCVWIDGETCANNY_HPP
