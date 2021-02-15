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

#ifndef COCVWIDGETINPAINTFUZZY_HPP
#define COCVWIDGETINPAINTFUZZY_HPP

#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "Process/OpenCV/COcvInpaintFuzzy.hpp"

class COcvWidgetInpaintFuzzy : public CProtocolTaskWidget
{
    public:

        COcvWidgetInpaintFuzzy(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetInpaintFuzzy(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvInpaintFuzzyParam>(pParam);;
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvInpaintFuzzyParam>();

            QLabel* pLabelRadius = new QLabel(QObject::tr("Neighborhood diameter"));
            m_pSpinRadius = new QSpinBox;
            m_pSpinRadius->setMinimum(1);
            m_pSpinRadius->setMaximum(256);
            m_pSpinRadius->setValue(m_pParam->m_radius);

            QLabel* pLabelMethod = new QLabel(QObject::tr("Method"));
            m_pComboMethod = new QComboBox;
            m_pComboMethod->addItem("One step");
            m_pComboMethod->addItem("Multi-step");
            m_pComboMethod->addItem("Iterative");

            if(m_pParam->m_method == cv::ft::ONE_STEP)
                m_pComboMethod->setCurrentIndex(0);
            else if(m_pParam->m_method == cv::ft::MULTI_STEP)
                m_pComboMethod->setCurrentIndex(1);
            else
                m_pComboMethod->setCurrentIndex(2);

            
            m_pLayout->addWidget(pLabelRadius, 0, 0);
            m_pLayout->addWidget(m_pSpinRadius, 0, 1);
            m_pLayout->addWidget(pLabelMethod, 1, 0);
            m_pLayout->addWidget(m_pComboMethod, 1, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_radius = m_pSpinRadius->value();

                int index = m_pComboMethod->currentIndex();
                switch(index)
                {
                    case 0: m_pParam->m_method = cv::ft::ONE_STEP; break;
                    case 1: m_pParam->m_method = cv::ft::MULTI_STEP; break;
                    case 2: m_pParam->m_method = cv::ft::ITERATIVE; break;
                }
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvInpaintFuzzyParam>  m_pParam = nullptr;
        QSpinBox*                               m_pSpinRadius = nullptr;
        QComboBox*                              m_pComboMethod = nullptr;
};

class COcvWidgetInpaintFuzzyFactory : public CWidgetFactory
{
    public:

        COcvWidgetInpaintFuzzyFactory()
        {
            m_name = QObject::tr("Inpaint fuzzy filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetInpaintFuzzy>(pParam);
        }
};

#endif // COCVWIDGETINPAINTFUZZY_HPP
