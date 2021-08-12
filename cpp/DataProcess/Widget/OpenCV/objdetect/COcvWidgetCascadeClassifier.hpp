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

#ifndef COCVWIDGETCASCADECLASSIFIER_HPP
#define COCVWIDGETCASCADECLASSIFIER_HPP

#include "Process/OpenCV/objdetect//COcvCascadeClassifier.hpp"
#include <QDir>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>

class COcvWidgetCascadeClassifier : public CWorkflowTaskWidget
{
    public:

        COcvWidgetCascadeClassifier(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCascadeClassifier(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCascadeClassifierParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCascadeClassifierParam>();

            m_dirHaarcascades = QDir::home();
            m_dirHaarcascades.cd("Ikomia/Resources/CascadeClassifier/haarcascades");

            m_dirLbpcascades = QDir::home();
            m_dirLbpcascades.cd("Ikomia/Resources/CascadeClassifier/lbpcascades");

            QStringList filters;
            filters << "*.xml";

            auto pGroupHaar = new QGroupBox(tr("Haarcascades"));
            m_pHaarLayout = new QVBoxLayout(pGroupHaar);
            foreach ( QString file, m_dirHaarcascades.entryList(filters, QDir::Files) )
            {
                auto pCheck = new QCheckBox(file);
                m_pHaarLayout->addWidget(pCheck);
                if(std::find_if(m_pParam->m_modelFiles.begin(), m_pParam->m_modelFiles.end(), [file](const std::string& str) { return str.find(file.toStdString()) != std::string::npos; }) != m_pParam->m_modelFiles.end())
                    pCheck->setChecked(true);
                else
                    pCheck->setChecked(false);
            }

            auto pGroupLbp = new QGroupBox(tr("Lbpcascades"));
            m_pLbpLayout = new QVBoxLayout(pGroupLbp);
            foreach ( QString file, m_dirLbpcascades.entryList(filters, QDir::Files) )
            {
                auto pCheck = new QCheckBox(file);
                m_pLbpLayout->addWidget(pCheck);
                if(std::find_if(m_pParam->m_modelFiles.begin(), m_pParam->m_modelFiles.end(), [file](const std::string& str) { return str.find(file.toStdString()) != std::string::npos; }) != m_pParam->m_modelFiles.end())
                    pCheck->setChecked(true);
                else
                    pCheck->setChecked(false);
            }

            m_pLayout->addWidget(pGroupHaar, 0, 0);
            m_pLayout->addWidget(pGroupLbp, 0, 1);            
        }

        void onApply() override
        {
            m_pParam->m_modelFiles.clear();
            for(int i=0; i<m_pHaarLayout->count(); ++i)
            {
                auto pCheck = qobject_cast<QCheckBox*>(m_pHaarLayout->itemAt(i)->widget());
                if(pCheck->isChecked())
                {
                    auto path = m_dirHaarcascades.absolutePath() + "/" + pCheck->text();
                    m_pParam->m_modelFiles.push_back(path.toStdString());
                }
            }
            for(int i=0; i<m_pLbpLayout->count(); ++i)
            {
                auto pCheck = qobject_cast<QCheckBox*>(m_pLbpLayout->itemAt(i)->widget());
                if(pCheck->isChecked())
                {
                    auto path = m_dirLbpcascades.absolutePath() + "/" + pCheck->text();
                    m_pParam->m_modelFiles.push_back(path.toStdString());
                }
            }
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvCascadeClassifierParam> m_pParam = nullptr;
        QVBoxLayout*    m_pHaarLayout = nullptr;
        QVBoxLayout*    m_pLbpLayout = nullptr;
        QDir            m_dirHaarcascades;
        QDir            m_dirLbpcascades;
};

class COcvWidgetCascadeClassifierFactory : public CWidgetFactory
{
    public:

        COcvWidgetCascadeClassifierFactory()
        {
            m_name = QObject::tr("Cascade Classifier Filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetCascadeClassifier>(pParam);
        }
};

#endif // COCVWIDGETCASCADECLASSIFIER_HPP
