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

#ifndef CWIDGETRGBHLSTHRESHOLD_H
#define CWIDGETRGBHLSTHRESHOLD_H

#include "Core/CWidgetFactory.hpp"
#include "Process/Core/CRgbHlsThreshold.h"

//----------------------------------//
//----- CWidgetRgbHlsThreshold -----//
//----------------------------------//
class CWidgetRgbHlsThreshold : public CWorkflowTaskWidget
{
    public:

        CWidgetRgbHlsThreshold(QWidget *parent = Q_NULLPTR);
        CWidgetRgbHlsThreshold(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR);

    protected:

        void    init();
        void    onApply() override;

    private:

        QPushButton*                            m_pClearBtn = nullptr;
        std::shared_ptr<CRgbHlsThresholdParam>  m_pParam = nullptr;
};

//-----------------------------------------//
//----- CWidgetRgbHlsThresholdFactory -----//
//-----------------------------------------//
class CWidgetRgbHlsThresholdFactory : public CWidgetFactory
{
    public:

        CWidgetRgbHlsThresholdFactory();
        ~CWidgetRgbHlsThresholdFactory() {}

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam);
};

#endif // CWIDGETRGBHLSTHRESHOLD_H
