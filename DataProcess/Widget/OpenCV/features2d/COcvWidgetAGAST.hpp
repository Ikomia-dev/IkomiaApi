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

#ifndef COCVWIDGETAGAST_HPP
#define COCVWIDGETAGAST_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvAGAST.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetAGAST : public CProtocolTaskWidget
{
    public:

        COcvWidgetAGAST(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetAGAST(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvAGASTParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvAGASTParam>();

            auto pSpinThresh = addSpin(0, tr("Threshold"), m_pParam->m_threshold);
            pSpinThresh->setRange(0, INT_MAX);
            auto pCombo = addCombo(1, tr("Type"));
            pCombo->addItem("AGAST_5_8", cv::AgastFeatureDetector::AGAST_5_8 );
            pCombo->addItem("AGAST_7_12d", cv::AgastFeatureDetector::AGAST_7_12d);
            pCombo->addItem("AGAST_7_12s", cv::AgastFeatureDetector::AGAST_7_12s);
            pCombo->addItem("OAST_9_16", cv::AgastFeatureDetector::OAST_9_16);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_type));
            auto pCheck = addCheck(2, tr("Non maximum suppression"), m_pParam->m_bNonmaxSuppression);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_threshold = pSpinThresh->value();
                m_pParam->m_type = pCombo->currentData().toInt();
                m_pParam->m_bNonmaxSuppression = pCheck->isChecked();

                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvAGASTParam> m_pParam = nullptr;
};

class COcvWidgetAGASTFactory : public CWidgetFactory
{
    public:

        COcvWidgetAGASTFactory()
        {
            m_name = QObject::tr("AGAST").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetAGAST>(pParam);
        }
};
#endif // COCVWIDGETAGAST_HPP
