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

class COcvWidgetFastNlMeansMulti : public CWorkflowTaskWidget
{
    public:

        COcvWidgetFastNlMeansMulti(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetFastNlMeansMulti(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
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
            m_pSpinTemporalSize = new QSpinBox;
            m_pSpinTemporalSize->setValue(m_pParam->m_temporalSize);
            m_pSpinTemporalSize->setSingleStep(2);
            m_pSpinTemporalSize->setRange(1, INT_MAX - 1);

            QLabel* pLabelH = new QLabel(QObject::tr("Filter strength"));
            m_pSpinH = new QDoubleSpinBox;
            m_pSpinH->setValue(m_pParam->m_h);
            m_pSpinH->setSingleStep(0.5);
            m_pSpinH->setRange(0, INT_MAX - 1);

            QLabel* pLabelBlockSize = new QLabel(QObject::tr("Block size"));
            m_pSpinBlockSize = new QDoubleSpinBox;
            m_pSpinBlockSize->setValue(m_pParam->m_blockSize);
            m_pSpinBlockSize->setSingleStep(2);
            m_pSpinBlockSize->setRange(1, INT_MAX - 1);

            QLabel* pLabelSearchSize = new QLabel(QObject::tr("Search size"));
            m_pSpinSearchSize = new QDoubleSpinBox;
            m_pSpinSearchSize->setValue(m_pParam->m_searchSize);
            m_pSpinSearchSize->setSingleStep(2);
            m_pSpinSearchSize->setRange(1, INT_MAX - 1);

            m_pLayout->addWidget(pLabelTemporalSize, 0, 0);
            m_pLayout->addWidget(m_pSpinTemporalSize, 0, 1);
            m_pLayout->addWidget(pLabelH, 1, 0);
            m_pLayout->addWidget(m_pSpinH, 1, 1);
            m_pLayout->addWidget(pLabelBlockSize, 2, 0);
            m_pLayout->addWidget(m_pSpinBlockSize, 2, 1);
            m_pLayout->addWidget(pLabelSearchSize, 3, 0);
            m_pLayout->addWidget(m_pSpinSearchSize, 3, 1);
        }

        void onApply() override
        {
            m_pParam->m_temporalSize = m_pSpinTemporalSize->value();
            m_pParam->m_h = m_pSpinH->value();
            m_pParam->m_blockSize = m_pSpinBlockSize->value();
            m_pParam->m_searchSize = m_pSpinSearchSize->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvFastNlMeansMultiParam>   m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinH = nullptr;
        QDoubleSpinBox* m_pSpinBlockSize = nullptr;
        QDoubleSpinBox* m_pSpinSearchSize = nullptr;
        QSpinBox*       m_pSpinTemporalSize = nullptr;
};

class COcvWidgetFastNlMeansMultiFactory : public CWidgetFactory
{
    public:

        COcvWidgetFastNlMeansMultiFactory()
        {
            m_name = QObject::tr("Non Local Means Multi Filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFastNlMeansMulti>(pParam);
        }
};

#endif // COCVWIDGETFastNlMeansMultiMULTI_HPP
