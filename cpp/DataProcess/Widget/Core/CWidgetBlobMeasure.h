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

#ifndef CWIDGETBLOBMEASURE_HPP
#define CWIDGETBLOBMEASURE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Core/CBlobMeasure.h"
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>

class CWidgetBlobMeasure : public CWorkflowTaskWidget
{
    public:

        CWidgetBlobMeasure(QWidget *parent = Q_NULLPTR);
        CWidgetBlobMeasure(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR);

    public slots:

        void            onItemChanged(QListWidgetItem* pItem);

    protected:

        virtual void    init();
        void            initConnections();

        void            fillMeasuresListView();

        int             getMeasureIndex(const std::string &name) const;

    private:

        std::shared_ptr<CBlobMeasureParam>  m_pParam = nullptr;
        QListWidget*                        m_pListView = nullptr;
        std::vector<CMeasure>               m_measures;
};

class CWidgetBlobMeasureFactory : public CWidgetFactory
{
    public:

        CWidgetBlobMeasureFactory()
        {
            m_name = QObject::tr("Objects measurements").toStdString();
        }
        ~CWidgetBlobMeasureFactory() {}

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CWidgetBlobMeasure>(pParam);
        }
};

#endif // CWIDGETBLOBMEASURE_HPP
