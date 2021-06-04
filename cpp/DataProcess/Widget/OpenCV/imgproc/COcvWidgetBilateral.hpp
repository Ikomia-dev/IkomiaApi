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

#ifndef COCVWIDGETBILATERAL_HPP
#define COCVWIDGETBILATERAL_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvBilateral.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
//#include <Widgets/QtPropertyBrowser/qtvariantproperty.h>
//#include <Widgets/QtPropertyBrowser/qtgroupboxpropertybrowser.h>

class COcvWidgetBilateral : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBilateral(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetBilateral(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBilateralParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBilateralParam>();

            QLabel* pLabelSpin = new QLabel(QObject::tr("Filter size d"));
            m_pSpin = new QSpinBox;
            m_pSpin->setValue(m_pParam->m_diameter);

            QLabel* pLabelSpinSpace = new QLabel(QObject::tr("Sigma spatial"));
            m_pDblSpinSpace = new QDoubleSpinBox;
            m_pDblSpinSpace->setValue(m_pParam->m_sigmaSpace);

            QLabel* pLabelSpinColor = new QLabel(QObject::tr("Sigma range"));
            m_pDblSpinColor = new QDoubleSpinBox;
            m_pDblSpinColor->setValue(m_pParam->m_sigmaColor);

            /*QtGroupBoxPropertyBrowser* m_pPropertyBrowser = new QtGroupBoxPropertyBrowser;
            QtVariantPropertyManager* m_pVariantManager = new QtVariantPropertyManager();
            QtVariantProperty* pProp = m_pVariantManager->addProperty(QtVariantPropertyManager::groupTypeId(), "Parameters");

            m_pFilterSize = m_pVariantManager->addProperty(QVariant::Int, "Filter size d");
            m_pFilterSize->setAttribute("minimum", 0);
            m_pFilterSize->setAttribute("maximum", 99);
            m_pFilterSize->setValue(m_pParam->m_diameter);
            pProp->addSubProperty(m_pFilterSize);

            m_pSigmaS = m_pVariantManager->addProperty(QVariant::Double, "Sigma spatial");
            m_pSigmaS->setAttribute("minimum", 0);
            m_pSigmaS->setAttribute("maximum", 99);
            m_pSigmaS->setAttribute("singleStep", 0.1);
            m_pSigmaS->setValue(m_pParam->m_sigmaSpace);
            pProp->addSubProperty(m_pSigmaS);

            m_pSigmaR = m_pVariantManager->addProperty(QVariant::Double, "Sigma range");
            m_pSigmaR->setAttribute("minimum", 0);
            m_pSigmaR->setAttribute("maximum", 99);
            m_pSigmaR->setAttribute("singleStep", 0.1);
            m_pSigmaR->setValue(m_pParam->m_sigmaColor);
            pProp->addSubProperty(m_pSigmaR);

            QtVariantEditorFactory* pFactory = new QtVariantEditorFactory;
            m_pPropertyBrowser->setFactoryForManager(m_pVariantManager, pFactory);
            m_pPropertyBrowser->addProperty(pProp);
            m_pPropertyBrowser->layout()->setSizeConstraint(QLayout::SetMinimumSize);*/

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_diameter = m_pSpin->value(); //m_pFilterSize->value().toDouble();
                m_pParam->m_sigmaSpace = m_pDblSpinSpace->value(); //m_pSigmaS->value().toDouble();
                m_pParam->m_sigmaColor = m_pDblSpinColor->value(); //m_pSigmaR->value().toDouble();
                emit doApplyProcess(m_pParam);
            });


            m_pLayout->addWidget(pLabelSpin, 0, 0);
            m_pLayout->addWidget(pLabelSpinSpace, 1, 0);
            m_pLayout->addWidget(pLabelSpinColor, 2, 0);
            m_pLayout->addWidget(m_pSpin, 0, 1);
            m_pLayout->addWidget(m_pDblSpinSpace, 1, 1);
            m_pLayout->addWidget(m_pDblSpinColor, 2, 1);
            //m_pLayout->addWidget(m_pPropertyBrowser, 0, 0);
            //m_pLayout->addWidget(m_pApplyBtn, 1, 0, 1, 1);

        }

    private:

        std::shared_ptr<COcvBilateralParam> m_pParam = nullptr;
        QSpinBox*                           m_pSpin = nullptr;
        QDoubleSpinBox*                     m_pDblSpinSpace = nullptr;
        QDoubleSpinBox*                     m_pDblSpinColor = nullptr;
        //QtVariantProperty*                  m_pFilterSize = nullptr;
        //QtVariantProperty*                  m_pSigmaS = nullptr;
        //QtVariantProperty*                  m_pSigmaR = nullptr;
};

class COcvWidgetBilateralFactory : public CWidgetFactory
{
    public:

        COcvWidgetBilateralFactory()
        {
            m_name = QObject::tr("Bilateral Filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBilateral>(pParam);
        }
};

#endif // COCVWIDGETBILATERAL_HPP
