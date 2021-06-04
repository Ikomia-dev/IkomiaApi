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

#ifndef COCVWIDGETDNNCLASSIFIER_HPP
#define COCVWIDGETDNNCLASSIFIER_HPP

#include "COcvWidgetDnnProcess.h"
#include "Process/OpenCV/dnn/COcvDnnClassifier.hpp"

class COcvWidgetDnnClassifier: public COcvWidgetDnnProcess
{
    public:

        COcvWidgetDnnClassifier(QWidget *parent = Q_NULLPTR): COcvWidgetDnnProcess(parent)
        {
            init();
        }
        COcvWidgetDnnClassifier(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : COcvWidgetDnnProcess(pParam, parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDnnClassifierParam>(pParam);
            init();
        }

        void init() override
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDnnClassifierParam>();

            auto pParam = std::dynamic_pointer_cast<COcvDnnClassifierParam>(m_pParam);
            assert(pParam);

            m_pComboNetType = addCombo(tr("Network type"));
            m_pComboNetType->addItem("AlexNet", COcvDnnClassifierParam::NetworkType::ALEXNET);
            m_pComboNetType->addItem("GoogleNet", COcvDnnClassifierParam::NetworkType::GOOGLENET);
            m_pComboNetType->addItem("Inception", COcvDnnClassifierParam::NetworkType::INCEPTION);
            m_pComboNetType->addItem("VGG", COcvDnnClassifierParam::NetworkType::VGG);
            m_pComboNetType->addItem("ResNet", COcvDnnClassifierParam::NetworkType::RESNET);
            m_pComboNetType->setCurrentIndex(m_pComboNetType->findData(pParam->m_netType));

            initConnections();
        }

    private:

        void    initConnections()
        {
            connect(m_pComboNetType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &COcvWidgetDnnClassifier::onNetworkTypeChanged);
        }

    private slots:

        void    onNetworkTypeChanged(int index)
        {
            Q_UNUSED(index);
            auto pParam = std::dynamic_pointer_cast<COcvDnnClassifierParam>(m_pParam);
            assert(pParam);
            pParam->m_netType = m_pComboNetType->currentData().toInt();
        }

    private:

        QComboBox*  m_pComboNetType = nullptr;
};

class COcvWidgetDnnClassifierFactory : public CWidgetFactory
{
    public:

        COcvWidgetDnnClassifierFactory()
        {
            m_name = QObject::tr("DNN classification").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDnnClassifier>(pParam);
        }
};

#endif // COCVWIDGETDNNCLASSIFIER_HPP
