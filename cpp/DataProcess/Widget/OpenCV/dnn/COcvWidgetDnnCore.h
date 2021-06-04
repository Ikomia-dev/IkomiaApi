/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef COCVWIDGETDNNCORE_H
#define COCVWIDGETDNNCORE_H

#include "Workflow/CWorkflowTaskWidget.h"
#include "Process/OpenCV/dnn/COcvDnnProcess.h"

class DATAPROCESSSHARED_EXPORT COcvWidgetDnnCore : public CWorkflowTaskWidget
{
    public:

        COcvWidgetDnnCore(QWidget *parent = Q_NULLPTR);
        COcvWidgetDnnCore(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR);

    protected:

        virtual void    init() override;

    private:

        void            initConnections();
        void            fillComboTarget(cv::dnn::Backend backend);
        QString         getTargetName(const cv::dnn::Target& target) const;

    protected:

        QComboBox*                          m_pComboBackend = nullptr;
        QComboBox*                          m_pComboTarget = nullptr;
        std::shared_ptr<COcvDnnCoreParam>   m_pParam = nullptr;
};

#endif // COCVWIDGETDNNCORE_H
