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

#ifndef COCVWIDGETHOUGHLINES_HPP
#define COCVWIDGETHOUGHLINES_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvHoughLines.hpp"

class COcvWidgetHoughLines : public CProtocolTaskWidget
{
    public:

        COcvWidgetHoughLines(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetHoughLines(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvHoughLinesParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvHoughLinesParam>();

            auto pCheckProba = addCheck(0, tr("Probabilistic method"), m_pParam->m_bProbabilistic);
            auto pSpinRho = addDoubleSpin(1, "Rho", m_pParam->m_rho, 1.0, DBL_MAX, 1.0, 1);
            auto pSpinTheta = addDoubleSpin(2, "Theta", m_pParam->m_theta, 0.0, 2*CV_PI/180.0, 1.0, 5);
            auto pSpinThreshold = addSpin(3, tr("Accumulator threshold"), m_pParam->m_threshold, 0, INT_MAX, 1);
            auto pSpinMinLength = addDoubleSpin(4, tr("Minimum length"), m_pParam->m_minLength, 0.0, DBL_MAX, 1.0, 1);
            auto pSpinMaxGap = addDoubleSpin(5, tr("Maximum gap"), m_pParam->m_maxGap, 0.0, DBL_MAX, 1.0, 1);
            

            pSpinMinLength->setEnabled(m_pParam->m_bProbabilistic);
            pSpinMaxGap->setEnabled(m_pParam->m_bProbabilistic);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_bProbabilistic = pCheckProba->isChecked();
                m_pParam->m_rho = pSpinRho->value();
                m_pParam->m_theta = pSpinTheta->value();
                m_pParam->m_threshold = pSpinThreshold->value();
                m_pParam->m_minLength = pSpinMinLength->value();
                m_pParam->m_maxGap = pSpinMaxGap->value();
                emit doApplyProcess(m_pParam);
            });

            connect(pCheckProba, &QCheckBox::clicked, [=](bool bChecked)
            {
                pSpinMinLength->setEnabled(bChecked);
                pSpinMaxGap->setEnabled(bChecked);
            });
        }

    private:

        std::shared_ptr<COcvHoughLinesParam> m_pParam = nullptr;
};

class COcvWidgetHoughLinesFactory : public CWidgetFactory
{
    public:

        COcvWidgetHoughLinesFactory()
        {
            m_name = QObject::tr("Hough lines").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetHoughLines>(pParam);
        }
};

#endif // COCVWIDGETHOUGHLINES_HPP
