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

#ifndef COCVWIDGETEXTRACTCHANNEL_HPP
#define COCVWIDGETEXTRACTCHANNEL_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvExtractChannel.hpp"

class COcvWidgetExtractChannel : public CWorkflowTaskWidget
{
    public:

        COcvWidgetExtractChannel(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetExtractChannel(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvExtractChannelParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvExtractChannelParam>();

            m_pSpinIndex = addSpin(0, tr("Channel index (zero-based)"), m_pParam->m_index, 0, 3, 1);
        }

        void onApply() override
        {
            m_pParam->m_index = m_pSpinIndex->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvExtractChannelParam> m_pParam = nullptr;
        QSpinBox* m_pSpinIndex = nullptr;
};

class COcvWidgetExtractChannelFactory : public CWidgetFactory
{
    public:

        COcvWidgetExtractChannelFactory()
        {
            m_name = "ocv_extract_channel";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetExtractChannel>(pParam);
        }
};

#endif // COCVWIDGETEXTRACTCHANNEL_HPP
