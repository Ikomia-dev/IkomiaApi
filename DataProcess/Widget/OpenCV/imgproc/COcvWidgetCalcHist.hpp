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

#ifndef COCVWIDGETCALCHIST_HPP
#define COCVWIDGETCALCHIST_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvCalcHist.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetCalcHist : public CProtocolTaskWidget
{
    public:

        COcvWidgetCalcHist(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCalcHist(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCalcHistParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCalcHistParam>();


            m_pSpin = new QSpinBox;
            m_pSpin->setMaximum(INT_MAX);
            m_pSpin->setValue(m_pParam->m_histSize[0]);

            QLabel* pLabelSpin = new QLabel(tr("Number of bins"));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_histSize[0] = m_pSpin->value();

                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabelSpin, 2, 0);
            m_pLayout->addWidget(m_pSpin, 2, 1);
            
        }

    private:

        std::shared_ptr<COcvCalcHistParam> m_pParam = nullptr;
        QSpinBox*                           m_pSpin = nullptr;
};

class COcvWidgetCalcHistFactory : public CWidgetFactory
{
    public:

        COcvWidgetCalcHistFactory()
        {
            m_name = QObject::tr("CalcHist").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetCalcHist>(pParam);
        }
};
#endif // COCVWIDGETCALCHIST_HPP
