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

#include "COcvWidgetDnnProcess.h"

COcvWidgetDnnProcess::COcvWidgetDnnProcess(QWidget *parent) : COcvWidgetDnnCore(parent)
{
    init();
}

COcvWidgetDnnProcess::COcvWidgetDnnProcess(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent) : COcvWidgetDnnCore(pParam, parent)
{
    m_pParam = std::dynamic_pointer_cast<COcvDnnProcessParam>(pParam);
    init();
}

void COcvWidgetDnnProcess::init()
{
    if(m_pParam == nullptr)
        m_pParam = std::make_shared<COcvDnnProcessParam>();

    auto pParam = std::dynamic_pointer_cast<COcvDnnProcessParam>(m_pParam);
    assert(pParam);

    m_pComboFramework = addCombo(tr("Framework"));
    m_pComboFramework->addItem(tr("TensorFlow"), COcvDnnProcessParam::TENSORFLOW);
    m_pComboFramework->addItem(tr("Caffe"), COcvDnnProcessParam::CAFFE);
    m_pComboFramework->addItem(tr("DarkNet"), COcvDnnProcessParam::DARKNET);
    m_pComboFramework->addItem(tr("Torch v7"), COcvDnnProcessParam::TORCH);
    m_pComboFramework->addItem(tr("ONNX"), COcvDnnProcessParam::ONNX);
    m_pComboFramework->setCurrentIndex(m_pComboFramework->findData(pParam->m_framework));

    m_pBrowseStructFile = addBrowseFile(tr("Structure file"), QString::fromStdString(pParam->m_structureFile), tr("Select network structure file"));
    m_pBrowseModelFile = addBrowseFile(tr("Model file"), QString::fromStdString(pParam->m_modelFile), tr("Select network model file"));
    m_pBrowseLabelFile = addBrowseFile(tr("Labels file"), QString::fromStdString(pParam->m_labelsFile), tr("Select labels file"));
    m_pBrowseLabelFile->setFilter("Labels file(*.txt)");

    initFramework(pParam->m_framework);
    initConnections();
}

void COcvWidgetDnnProcess::clearFiles()
{
    m_pBrowseStructFile->clear();
    m_pBrowseModelFile->clear();
    m_pBrowseLabelFile->clear();
}

void COcvWidgetDnnProcess::onFrameworkChanged(int index)
{
    clearFiles();
    initFramework(m_pComboFramework->itemData(index).toInt());
    m_pParam->m_bUpdate = true;
}

void COcvWidgetDnnProcess::onApply()
{
    auto pParam = std::dynamic_pointer_cast<COcvDnnProcessParam>(m_pParam);
    assert(pParam);

    pParam->m_framework = m_pComboFramework->currentData().toInt();
    pParam->m_modelFile = m_pBrowseModelFile->getPath().toStdString();
    pParam->m_structureFile = m_pBrowseStructFile->getPath().toStdString();
    pParam->m_labelsFile = m_pBrowseLabelFile->getPath().toStdString();
    emit doApplyProcess(pParam);
}

void COcvWidgetDnnProcess::initConnections()
{
    connect(m_pComboFramework, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &COcvWidgetDnnProcess::onFrameworkChanged);
    connect(m_pBrowseStructFile, &CBrowseFileWidget::selectedFileChanged, [&](const QString& filePath)
    {
        auto pParam = std::dynamic_pointer_cast<COcvDnnProcessParam>(m_pParam);
        assert(pParam);

        if(filePath.toStdString() != pParam->m_structureFile)
            m_pParam->m_bUpdate = true;
    });
    connect(m_pBrowseModelFile, &CBrowseFileWidget::selectedFileChanged, [&](const QString& filePath)
    {
        auto pParam = std::dynamic_pointer_cast<COcvDnnProcessParam>(m_pParam);
        assert(pParam);

        if(filePath.toStdString() != pParam->m_modelFile)
            m_pParam->m_bUpdate = true;
    });
}

void COcvWidgetDnnProcess::initFramework(int framework)
{
    switch(framework)
    {
        case COcvDnnProcessParam::TENSORFLOW:
            m_pBrowseStructFile->setFilter("Structure file(*.pbtxt)");
            m_pBrowseModelFile->setFilter("Model file(*.pb)");
            break;
        case COcvDnnProcessParam::CAFFE:
            m_pBrowseStructFile->setFilter("Structure file(*.prototxt)");
            m_pBrowseModelFile->setFilter("Model file(*.caffemodel)");
            break;
        case COcvDnnProcessParam::DARKNET:
            m_pBrowseStructFile->setFilter("Structure file(*.cfg)");
            m_pBrowseModelFile->setFilter("Model file(*.weights)");
            break;
        case COcvDnnProcessParam::TORCH:
            m_pBrowseStructFile->setFilter("");
            m_pBrowseModelFile->setFilter("Model file(*.t7 *.net)");
            break;
        case COcvDnnProcessParam::ONNX:
            m_pBrowseStructFile->setFilter("");
            m_pBrowseModelFile->setFilter("Model file(*.onnx)");
            break;
    }
}
