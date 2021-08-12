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

#ifndef COCVWIDGETQRCODEDETECTOR_HPP
#define COCVWIDGETQRCODEDETECTOR_HPP
#include "Process/OpenCV/objdetect/COcvQRCodeDetector.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetQRCodeDetector : public CWorkflowTaskWidget
{
    public:

        COcvWidgetQRCodeDetector(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetQRCodeDetector(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvQRCodeDetectorParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvQRCodeDetectorParam>();

            m_pSpinEpsX = addDoubleSpin(0, tr("Eps x"), m_pParam->m_eps_x, 0, DBL_MAX, 0.1);
            m_pSpinEpsY = addDoubleSpin(1, tr("Eps y"), m_pParam->m_eps_y, 0, DBL_MAX, 0.1);
        }

        void onApply() override
        {
            m_pParam->m_eps_x = m_pSpinEpsX->value();
            m_pParam->m_eps_y = m_pSpinEpsY->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvQRCodeDetectorParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinEpsX = nullptr;
        QDoubleSpinBox* m_pSpinEpsY = nullptr;
};

class COcvWidgetQRCodeDetectorFactory : public CWidgetFactory
{
    public:

        COcvWidgetQRCodeDetectorFactory()
        {
            m_name = QObject::tr("QRCodeDetector").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetQRCodeDetector>(pParam);
        }
};

#endif // COCVWIDGETQRCODEDETECTOR_HPP
