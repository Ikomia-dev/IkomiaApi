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

#include "CWorkflowTaskWidget.h"
#include "PythonThread.hpp"

CWorkflowTaskWidget::CWorkflowTaskWidget(QWidget *parent) : QWidget(parent)
{
    // Internal grid layout -> for internal process or C++ plugin
    m_pLayout = new QGridLayout;

    // General Apply button
    m_pApplyBtn = new QPushButton(tr("Apply"));
    m_pApplyBtn->setObjectName("CProcessBtn");
    m_pApplyBtn->setDefault(true);

    // Create layout to keep gridlayout widgets close to each other
    m_pContainerLayout = new QVBoxLayout;
    m_pContainerLayout->addLayout(m_pLayout);
    m_pContainerLayout->addStretch(0);

    // Create widget containing gridlayout
    QWidget* pWidget = new QWidget;
    pWidget->setObjectName("CWidget");
    pWidget->setLayout(m_pContainerLayout);

    // Put this widget in a scroll area
    m_pScrollArea = new QScrollArea;
    m_pScrollArea->setWidgetResizable(true);
    m_pScrollArea->setWidget(pWidget);

    // Create main layout containing scroll area and apply button
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setContentsMargins(0,0,0,0);
    pLayout->setSpacing(10);
    pLayout->addWidget(m_pScrollArea);
    pLayout->addWidget(m_pApplyBtn);
    setLayout(pLayout);

    connect(m_pApplyBtn, &QPushButton::clicked, this, &CWorkflowTaskWidget::onApply);
}

void CWorkflowTaskWidget::setApplyBtnHidden(bool bHidden)
{
    m_pApplyBtn->setHidden(bHidden);
}

QLineEdit* CWorkflowTaskWidget::addEdit(int row, const QString& title, const QString& value)
{
    auto pLabel = new QLabel(title);
    auto pLineEdit = new QLineEdit;
    pLineEdit->setText(value);
    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pLineEdit, row, 1);
    return pLineEdit;
}

QLineEdit* CWorkflowTaskWidget::addEdit(const QString& title, const QString& value)
{
    int nextPos = m_pLayout->rowCount();
    return addEdit(nextPos, title, value);
}

QComboBox *CWorkflowTaskWidget::addCombo(int row, const QString &title)
{
    auto pLabel = new QLabel(title);
    auto pCombo = new QComboBox;
    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pCombo, row, 1);
    return pCombo;
}

QComboBox *CWorkflowTaskWidget::addCombo(const QString &title)
{
    int nextPos = m_pLayout->rowCount();
    return addCombo(nextPos, title);
}

QSpinBox *CWorkflowTaskWidget::addSpin(int row, const QString &title, int value, int min, int max, int step)
{
    auto pLabel = new QLabel(title);
    auto pSpin = new QSpinBox;
    pSpin->setRange(min, max);
    pSpin->setSingleStep(step);
    pSpin->setValue(value);

    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pSpin, row, 1);
    return pSpin;
}

QSpinBox *CWorkflowTaskWidget::addSpin(const QString &title, int value, int min, int max, int step)
{
    int nextPos = m_pLayout->rowCount();
    return addSpin(nextPos, title, value, min, max, step);
}

QDoubleSpinBox *CWorkflowTaskWidget::addDoubleSpin(int row, const QString &title, double value, double min, double max, double step, int decimals)
{
    auto pLabel = new QLabel(title);
    auto pSpin = new QDoubleSpinBox;
    pSpin->setDecimals(decimals);
    pSpin->setRange(min, max);
    pSpin->setValue(value);
    pSpin->setSingleStep(step);

    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pSpin, row, 1);
    return pSpin;
}

QDoubleSpinBox *CWorkflowTaskWidget::addDoubleSpin(const QString &title, double value, double min, double max, double step, int decimals)
{
    int nextPos = m_pLayout->rowCount();
    return addDoubleSpin(nextPos, title, value, min, max, step, decimals);
}

QCheckBox *CWorkflowTaskWidget::addCheck(int row, const QString &title, bool isChecked)
{
    QCheckBox* pCheck = new QCheckBox(title);
    pCheck->setChecked(isChecked);
    m_pLayout->addWidget(pCheck, row, 0, 1, 2);

    return pCheck;
}

QCheckBox *CWorkflowTaskWidget::addCheck(const QString &title, bool isChecked)
{
    int nextPos = m_pLayout->rowCount();
    return addCheck(nextPos, title, isChecked);
}

QRadioButton *CWorkflowTaskWidget::addRadio(int row, const QString &title, bool isChecked)
{
    QRadioButton* pRadio = new QRadioButton(title);
    pRadio->setChecked(isChecked);
    m_pLayout->addWidget(pRadio, row, 0, 1, 2);

    return pRadio;
}

QRadioButton *CWorkflowTaskWidget::addRadio(const QString &title, bool isChecked)
{
    int nextPos = m_pLayout->rowCount();
    return addRadio(nextPos, title, isChecked);
}

QSlider *CWorkflowTaskWidget::addSlider(int row, const QString &title, int value, int min, int max, int step)
{
    auto pLabel = new QLabel(title);
    auto pSlider = new QSlider(Qt::Horizontal);
    pSlider->setRange(min, max);
    pSlider->setSingleStep(step);
    pSlider->setValue(value);

    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pSlider, row, 1);
    return pSlider;
}

QSlider *CWorkflowTaskWidget::addSlider(const QString &title, int value, int min, int max, int step)
{
    int nextPos = m_pLayout->rowCount();
    return addSlider(nextPos, title, value, min, max, step);
}

CColorPushButton *CWorkflowTaskWidget::addColorButton(int row, const QString &title, const QColor &color)
{
    auto pLabel = new QLabel(title);
    auto pBtn = new CColorPushButton(title, color);
    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pBtn, row, 1);
    return pBtn;
}

CColorPushButton *CWorkflowTaskWidget::addColorButton(const QString &title, const QColor &color)
{
    int nextPos = m_pLayout->rowCount();
    return addColorButton(nextPos, title, color);
}

CBrowseFileWidget *CWorkflowTaskWidget::addBrowseFile(int row, const QString &title, const QString &path, const QString &tooltip)
{
    auto pLabel = new QLabel(title);
    auto pBrowseWidget = new CBrowseFileWidget(path, tooltip);
    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pBrowseWidget, row, 1);
    return pBrowseWidget;
}

CBrowseFileWidget *CWorkflowTaskWidget::addBrowseFile(const QString &title, const QString &path, const QString &tooltip)
{
    int nextPos = m_pLayout->rowCount();
    return addBrowseFile(nextPos, title, path, tooltip);
}

CBrowseFileWidget *CWorkflowTaskWidget::addBrowseFolder(int row, const QString &title, const QString &path, const QString &tooltip)
{
    auto pLabel = new QLabel(title);
    auto pBrowseWidget = new CBrowseFileWidget(path, tooltip, QFileDialog::FileMode::Directory);
    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pBrowseWidget, row, 1);
    return pBrowseWidget;
}

CBrowseFileWidget *CWorkflowTaskWidget::addBrowseFolder(const QString &title, const QString &path, const QString &tooltip)
{
    int nextPos = m_pLayout->rowCount();
    return addBrowseFolder(nextPos, title, path, tooltip);
}

#include "moc_CWorkflowTaskWidget.cpp"
