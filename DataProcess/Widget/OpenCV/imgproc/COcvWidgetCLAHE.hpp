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

#ifndef COCVWIDGETCLAHE_HPP
#define COCVWIDGETCLAHE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvCLAHE.hpp"
#include <QSpinBox>

class COcvWidgetCLAHE : public CProtocolTaskWidget
{
    public:

        COcvWidgetCLAHE(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCLAHE(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCLAHEParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCLAHEParam>();

            auto pSpinClipLimit = addDoubleSpin(0, tr("Threshold for contrast limiting"), m_pParam->m_clipLimit);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_clipLimit = pSpinClipLimit->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvCLAHEParam> m_pParam = nullptr;
};

class COcvWidgetCLAHEFactory : public CWidgetFactory
{
    public:

        COcvWidgetCLAHEFactory()
        {
            m_name = QObject::tr("CLAHE").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetCLAHE>(pParam);
        }
};

#endif // COCVWIDGETCLAHE_HPP
