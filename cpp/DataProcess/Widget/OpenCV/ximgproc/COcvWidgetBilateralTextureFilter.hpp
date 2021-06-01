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

#ifndef COCVWIDGETBILATERALTEXTUREFILTER_HPP
#define COCVWIDGETBILATERALTEXTUREFILTER_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvBilateralTextureFilter.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetBilateralTextureFilter : public CProtocolTaskWidget
{
    public:

        COcvWidgetBilateralTextureFilter(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetBilateralTextureFilter(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBilateralTextureFilterParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBilateralTextureFilterParam>();

            QLabel* pLabelSpin = new QLabel(QObject::tr("Filter size d"));
            m_pSpin = new QSpinBox;
            m_pSpin->setValue(m_pParam->m_fr);
            m_pSpin->setRange(0, 100);

            QLabel* pLabelSpinIter = new QLabel(QObject::tr("Iterations"));
            m_pDblSpinIter = new QDoubleSpinBox;
            m_pDblSpinIter->setValue(m_pParam->m_numIter);
            m_pDblSpinIter->setRange(0, 1000);

            QLabel* pLabelSpinAlpha = new QLabel(QObject::tr("Sigma alpha"));
            m_pDblSpinAlpha = new QDoubleSpinBox;
            m_pDblSpinAlpha->setRange(-1, 100);
            m_pDblSpinAlpha->setValue(m_pParam->m_sigmaAlpha);

            QLabel* pLabelSpinAvg = new QLabel(QObject::tr("Sigma avg"));
            m_pDblSpinAvg = new QDoubleSpinBox;
            m_pDblSpinAvg->setRange(-1, 100);
            m_pDblSpinAvg->setValue(m_pParam->m_sigmaAvg);

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_fr = m_pSpin->value();
                m_pParam->m_numIter = m_pDblSpinIter->value();
                m_pParam->m_sigmaAlpha = m_pDblSpinAlpha->value();
                m_pParam->m_sigmaAvg = m_pDblSpinAvg->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabelSpin, 0, 0);
            m_pLayout->addWidget(m_pSpin, 0, 1);

            m_pLayout->addWidget(pLabelSpinIter, 1, 0);
            m_pLayout->addWidget(m_pDblSpinIter, 1, 1);

            m_pLayout->addWidget(pLabelSpinAlpha, 2, 0);
            m_pLayout->addWidget(m_pDblSpinAlpha, 2, 1);

            m_pLayout->addWidget(pLabelSpinAvg, 3, 0);
            m_pLayout->addWidget(m_pDblSpinAvg, 3, 1);

            
        }

    private:

        std::shared_ptr<COcvBilateralTextureFilterParam> m_pParam = nullptr;
        QSpinBox*                           m_pSpin = nullptr;
        QDoubleSpinBox*                     m_pDblSpinIter = nullptr;
        QDoubleSpinBox*                     m_pDblSpinAlpha = nullptr;
        QDoubleSpinBox*                     m_pDblSpinAvg = nullptr;
};

class COcvWidgetBilateralTextureFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetBilateralTextureFilterFactory()
        {
            m_name = QObject::tr("Bilateral texture filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBilateralTextureFilter>(pParam);
        }
};

#endif // COCVWIDGETBILATERALTEXTUREFILTER_HPP
