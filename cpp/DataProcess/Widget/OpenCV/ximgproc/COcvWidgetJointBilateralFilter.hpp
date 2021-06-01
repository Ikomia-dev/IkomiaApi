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

#ifndef COCVWIDGETJOINTJointBilateralFilterFILTER_HPP
#define COCVWIDGETJOINTJointBilateralFilterFILTER_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvJointBilateralFilter.hpp"

class COcvWidgetJointBilateralFilter : public CProtocolTaskWidget
{
    public:

        COcvWidgetJointBilateralFilter(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetJointBilateralFilter(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvJointBilateralFilterParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvJointBilateralFilterParam>();

            auto pSpinDiameter = addSpin(0, tr("Diameter"), m_pParam->m_diameter);
            auto pSpinSigmaColor = addDoubleSpin(1, tr("Sigma color"), m_pParam->m_sigmaColor);
            auto pSpinSigmaSpace = addDoubleSpin(2, tr("Sigma space"), m_pParam->m_sigmaSpace);

            auto pComboBorder = addCombo(3, tr("Border type"));
            pComboBorder->addItem("BORDER_DEFAULT", cv::BORDER_DEFAULT);
            pComboBorder->addItem("BORDER_CONSTANT", cv::BORDER_CONSTANT);
            pComboBorder->addItem("BORDER_REFLECT", cv::BORDER_REFLECT);
            pComboBorder->addItem("BORDER_WRAP", cv::BORDER_WRAP);
            pComboBorder->addItem("BORDER_REFLECT_101", cv::BORDER_REFLECT_101);
            pComboBorder->addItem("BORDER_TRANSPARENT", cv::BORDER_TRANSPARENT);
            pComboBorder->addItem("BORDER_ISOLATED", cv::BORDER_ISOLATED);
            pComboBorder->addItem("BORDER_REPLICATE", cv::BORDER_REPLICATE);
            pComboBorder->setCurrentIndex(pComboBorder->findData(m_pParam->m_borderType));

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_diameter = pSpinDiameter->value();
                m_pParam->m_sigmaColor = pSpinSigmaColor->value();
                m_pParam->m_sigmaSpace = pSpinSigmaSpace->value();
                m_pParam->m_borderType = pComboBorder->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvJointBilateralFilterParam> m_pParam = nullptr;
};

class COcvWidgetJointBilateralFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetJointBilateralFilterFactory()
        {
            m_name = QObject::tr("Joint Bilateral Filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetJointBilateralFilter>(pParam);
        }
};

#endif // COCVWIDGETJOINTJointBilateralFilterFILTER_HPP
