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

#include "CWidgetBlobMeasure.h"

CWidgetBlobMeasure::CWidgetBlobMeasure(QWidget *parent) : CProtocolTaskWidget(parent)
{
    init();
}

CWidgetBlobMeasure::CWidgetBlobMeasure(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent) : CProtocolTaskWidget(parent)
{
    m_pParam = std::dynamic_pointer_cast<CBlobMeasureParam>(pParam);
    init();
}

void CWidgetBlobMeasure::onItemChanged(QListWidgetItem *pItem)
{
    int index = m_pListView->row(pItem);
    assert(index >= 0 && index < static_cast<int>(m_measures.size()));

    if(pItem->checkState() == Qt::Checked)
    {
        auto measureIndex = getMeasureIndex(m_measures[index].m_name);
        if(measureIndex == -1)
            m_pParam->m_measures.push_back(m_measures[index]);
    }
    else
    {
        auto measureIndex = getMeasureIndex(m_measures[index].m_name);
        if(measureIndex != -1)
            m_pParam->m_measures.erase(m_pParam->m_measures.begin() + measureIndex);
    }
}

void CWidgetBlobMeasure::init()
{
    if(m_pParam == nullptr)
        m_pParam = std::make_shared<CBlobMeasureParam>();

    m_pListView = new QListWidget;
    fillMeasuresListView();

    auto pGroupLayout = new QVBoxLayout;
    pGroupLayout->addWidget(m_pListView);

    auto pGroupBox = new QGroupBox(tr("Available measures"));
    pGroupBox->setLayout(pGroupLayout);

    m_pLayout->addWidget(pGroupBox);
    
    initConnections();
}

void CWidgetBlobMeasure::initConnections()
{
    connect(m_pListView, &QListWidget::itemChanged, this, &CWidgetBlobMeasure::onItemChanged);
    connect(m_pApplyBtn, &QPushButton::clicked, [&]{ emit doApplyProcess(m_pParam); } );
}

void CWidgetBlobMeasure::fillMeasuresListView()
{
    QStringList names;
    m_measures = CMeasure::getAvailableMeasures();

    for(size_t i=0; i<m_measures.size(); ++i)
        names.push_back(QString::fromStdString(m_measures[i].m_name));

    m_pListView->addItems(names);

    for(int i=0; i<m_pListView->count(); ++i)
    {
        auto pItem = m_pListView->item(i);
        pItem->setFlags(pItem->flags() | Qt::ItemIsUserCheckable);

        if(getMeasureIndex(m_measures[i].m_name) != -1)
            pItem->setCheckState(Qt::Checked);
        else
            pItem->setCheckState(Qt::Unchecked);
    }
}

int CWidgetBlobMeasure::getMeasureIndex(const std::string &name) const
{
    for(int i=0; i<(int)m_pParam->m_measures.size(); ++i)
    {
        if(m_pParam->m_measures[i].m_name == name)
            return i;
    }
    return -1;
}
