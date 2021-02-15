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

#ifndef COCVWIDGETFastNlMeansMultiMULTI_HPP
#define COCVWIDGETFastNlMeansMultiMULTI_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/photo/COcvFastNlMeansMulti.hpp"

class COcvWidgetFastNlMeansMulti : public CProtocolTaskWidget
{
    public:

        COcvWidgetFastNlMeansMulti(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetFastNlMeansMulti(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvFastNlMeansMultiParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFastNlMeansMultiParam>();

            QLabel* pLabelTemporalSize = new QLabel(QObject::tr("Temporal windows size"));
            auto pSpinTemporalSize = new QSpinBox;
            pSpinTemporalSize->setValue(m_pParam->m_temporalSize);
            pSpinTemporalSize->setSingleStep(2);
            pSpinTemporalSize->setRange(1, INT_MAX - 1);

            QLabel* pLabelH = new QLabel(QObject::tr("Filter strength"));
            auto pSpinH = new QDoubleSpinBox;
            pSpinH->setValue(m_pParam->m_h);
            pSpinH->setSingleStep(0.5);
            pSpinH->setRange(0, INT_MAX - 1);

            QLabel* pLabelBlockSize = new QLabel(QObject::tr("Block size"));
            auto pSpinBlockSize = new QDoubleSpinBox;
            pSpinBlockSize->setValue(m_pParam->m_blockSize);
            pSpinBlockSize->setSingleStep(2);
            pSpinBlockSize->setRange(1, INT_MAX - 1);

            QLabel* pLabelSearchSize = new QLabel(QObject::tr("Search size"));
            auto pSpinSearchSize = new QDoubleSpinBox;
            pSpinSearchSize->setValue(m_pParam->m_searchSize);
            pSpinSearchSize->setSingleStep(2);
            pSpinSearchSize->setRange(1, INT_MAX - 1);

            
            m_pLayout->addWidget(pLabelTemporalSize, 0, 0);
            m_pLayout->addWidget(pSpinTemporalSize, 0, 1);
            m_pLayout->addWidget(pLabelH, 1, 0);
            m_pLayout->addWidget(pSpinH, 1, 1);
            m_pLayout->addWidget(pLabelBlockSize, 2, 0);
            m_pLayout->addWidget(pSpinBlockSize, 2, 1);
            m_pLayout->addWidget(pLabelSearchSize, 3, 0);
            m_pLayout->addWidget(pSpinSearchSize, 3, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_temporalSize = pSpinTemporalSize->value();
                m_pParam->m_h = pSpinH->value();
                m_pParam->m_blockSize = pSpinBlockSize->value();
                m_pParam->m_searchSize = pSpinSearchSize->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvFastNlMeansMultiParam>   m_pParam = nullptr;
};

class COcvWidgetFastNlMeansMultiFactory : public CWidgetFactory
{
    public:

        COcvWidgetFastNlMeansMultiFactory()
        {
            m_name = QObject::tr("Non Local Means Multi Filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFastNlMeansMulti>(pParam);
        }
};

#endif // COCVWIDGETFastNlMeansMultiMULTI_HPP
