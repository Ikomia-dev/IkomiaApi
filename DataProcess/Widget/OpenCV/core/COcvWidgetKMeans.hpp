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

#ifndef COCVWIDGETKMEANS_HPP
#define COCVWIDGETKMEANS_HPP

#include "Process/OpenCV/core/COcvKMeans.hpp"
#include <QSpinBox>
#include <QGridLayout>
#include <QComboBox>

class COcvWidgetKMeans : public CProtocolTaskWidget
{
    public:

        COcvWidgetKMeans(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetKMeans(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvKMeansParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvKMeansParam>();

            auto pLabelClassNb = new QLabel(tr("Number of classes"));
            auto pSpinClassNb = new QSpinBox;
            pSpinClassNb->setRange(1, 255);
            pSpinClassNb->setValue(m_pParam->m_k);

            auto pLabelTermCriteria = new QLabel(tr("Stop condition"));
            auto pComboTermCriteria = new QComboBox;
            pComboTermCriteria->addItem(tr("Precision"), cv::TermCriteria::EPS);
            pComboTermCriteria->addItem(tr("Number of iteration"), cv::TermCriteria::MAX_ITER);
            pComboTermCriteria->addItem(tr("Precision or iteration"), cv::TermCriteria::EPS+cv::TermCriteria::MAX_ITER);
            pComboTermCriteria->setCurrentIndex(pComboTermCriteria->findData(m_pParam->m_termType));

            auto pLabelTermEps = new QLabel(tr("Precision"));
            auto pEditPrecision = new QLineEdit;
            pEditPrecision->setText(QString::number(m_pParam->m_termEpsilon));

            auto pLabelTermIter = new QLabel(tr("Number of iterations"));
            auto pSpinTermIter = new QSpinBox;
            pSpinTermIter->setRange(1, 1000);
            pSpinTermIter->setValue(m_pParam->m_termMaxCount);

            auto pLabelAttempts = new QLabel(tr("Number of attempts"));
            auto pSpinAttempts = new QSpinBox;
            pSpinAttempts->setRange(1, 20);
            pSpinAttempts->setValue(m_pParam->m_attempts);

            auto pLabelInitMethod = new QLabel(tr("Initialization method"));
            auto pComboInitMethod = new QComboBox;
            pComboInitMethod->addItem(tr("Random centers"), cv::KmeansFlags::KMEANS_RANDOM_CENTERS);
            pComboInitMethod->addItem(tr("K-means++"), cv::KmeansFlags::KMEANS_PP_CENTERS);
            pComboInitMethod->setCurrentIndex(pComboInitMethod->findData(m_pParam->m_flags));

            
            m_pLayout->addWidget(pLabelClassNb, 0, 0);
            m_pLayout->addWidget(pSpinClassNb, 0, 1);
            m_pLayout->addWidget(pLabelTermCriteria, 1, 0);
            m_pLayout->addWidget(pComboTermCriteria, 1, 1);
            m_pLayout->addWidget(pLabelTermEps, 2, 0);
            m_pLayout->addWidget(pEditPrecision, 2, 1);
            m_pLayout->addWidget(pLabelTermIter, 3, 0);
            m_pLayout->addWidget(pSpinTermIter, 3, 1);
            m_pLayout->addWidget(pLabelAttempts, 4, 0);
            m_pLayout->addWidget(pSpinAttempts, 4, 1);
            m_pLayout->addWidget(pLabelInitMethod, 5, 0);
            m_pLayout->addWidget(pComboInitMethod, 5, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_k = pSpinClassNb->value();
                m_pParam->m_termType = pComboTermCriteria->currentData().toInt();
                m_pParam->m_termEpsilon = pEditPrecision->text().toDouble();
                m_pParam->m_termMaxCount = pSpinTermIter->value();
                m_pParam->m_attempts = pSpinAttempts->value();
                m_pParam->m_flags = pComboInitMethod->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });

        }

    private:

        std::shared_ptr<COcvKMeansParam>    m_pParam = nullptr;
};

class COcvWidgetKMeansFactory : public CWidgetFactory
{
    public:

        COcvWidgetKMeansFactory()
        {
            m_name = QObject::tr("K-means").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetKMeans>(pParam);
        }
};

#endif // COCVWIDGETKMEANS_HPP
