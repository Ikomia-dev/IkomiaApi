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

class COcvWidgetKMeans : public CWorkflowTaskWidget
{
    public:

        COcvWidgetKMeans(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetKMeans(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
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
            m_pSpinClassNb = new QSpinBox;
            m_pSpinClassNb->setRange(1, 255);
            m_pSpinClassNb->setValue(m_pParam->m_k);

            auto pLabelTermCriteria = new QLabel(tr("Stop condition"));
            m_pComboTermCriteria = new QComboBox;
            m_pComboTermCriteria->addItem(tr("Precision"), cv::TermCriteria::EPS);
            m_pComboTermCriteria->addItem(tr("Number of iteration"), cv::TermCriteria::MAX_ITER);
            m_pComboTermCriteria->addItem(tr("Precision or iteration"), cv::TermCriteria::EPS+cv::TermCriteria::MAX_ITER);
            m_pComboTermCriteria->setCurrentIndex(m_pComboTermCriteria->findData(m_pParam->m_termType));

            auto pLabelTermEps = new QLabel(tr("Precision"));
            m_pEditPrecision = new QLineEdit;
            m_pEditPrecision->setText(QString::number(m_pParam->m_termEpsilon));

            auto pLabelTermIter = new QLabel(tr("Number of iterations"));
            m_pSpinTermIter = new QSpinBox;
            m_pSpinTermIter->setRange(1, 1000);
            m_pSpinTermIter->setValue(m_pParam->m_termMaxCount);

            auto pLabelAttempts = new QLabel(tr("Number of attempts"));
            m_pSpinAttempts = new QSpinBox;
            m_pSpinAttempts->setRange(1, 20);
            m_pSpinAttempts->setValue(m_pParam->m_attempts);

            auto pLabelInitMethod = new QLabel(tr("Initialization method"));
            m_pComboInitMethod = new QComboBox;
            m_pComboInitMethod->addItem(tr("Random centers"), cv::KmeansFlags::KMEANS_RANDOM_CENTERS);
            m_pComboInitMethod->addItem(tr("K-means++"), cv::KmeansFlags::KMEANS_PP_CENTERS);
            m_pComboInitMethod->setCurrentIndex(m_pComboInitMethod->findData(m_pParam->m_flags));
            
            m_pLayout->addWidget(pLabelClassNb, 0, 0);
            m_pLayout->addWidget(m_pSpinClassNb, 0, 1);
            m_pLayout->addWidget(pLabelTermCriteria, 1, 0);
            m_pLayout->addWidget(m_pComboTermCriteria, 1, 1);
            m_pLayout->addWidget(pLabelTermEps, 2, 0);
            m_pLayout->addWidget(m_pEditPrecision, 2, 1);
            m_pLayout->addWidget(pLabelTermIter, 3, 0);
            m_pLayout->addWidget(m_pSpinTermIter, 3, 1);
            m_pLayout->addWidget(pLabelAttempts, 4, 0);
            m_pLayout->addWidget(m_pSpinAttempts, 4, 1);
            m_pLayout->addWidget(pLabelInitMethod, 5, 0);
            m_pLayout->addWidget(m_pComboInitMethod, 5, 1);
        }

        void onApply() override
        {
            m_pParam->m_k = m_pSpinClassNb->value();
            m_pParam->m_termType = m_pComboTermCriteria->currentData().toInt();
            m_pParam->m_termEpsilon = m_pEditPrecision->text().toDouble();
            m_pParam->m_termMaxCount = m_pSpinTermIter->value();
            m_pParam->m_attempts = m_pSpinAttempts->value();
            m_pParam->m_flags = m_pComboInitMethod->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvKMeansParam>    m_pParam = nullptr;
        QSpinBox*   m_pSpinClassNb = nullptr;
        QSpinBox*   m_pSpinTermIter = nullptr;
        QSpinBox*   m_pSpinAttempts = nullptr;
        QComboBox*  m_pComboTermCriteria = nullptr;
        QComboBox*  m_pComboInitMethod = nullptr;
        QLineEdit*  m_pEditPrecision = nullptr;
};

class COcvWidgetKMeansFactory : public CWidgetFactory
{
    public:

        COcvWidgetKMeansFactory()
        {
            m_name = "ocv_kmeans";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetKMeans>(pParam);
        }
};

#endif // COCVWIDGETKMEANS_HPP
