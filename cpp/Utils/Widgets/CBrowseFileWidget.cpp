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

#include "CBrowseFileWidget.h"
#include <QHBoxLayout>

CBrowseFileWidget::CBrowseFileWidget(QWidget *pParent) : QWidget(pParent)
{
    init();
    initConnections();
}

CBrowseFileWidget::CBrowseFileWidget(const QString& file, const QString& tooltip, QFileDialog::FileMode mode, QWidget *pParent)
    : QWidget(pParent)
{
    m_filePath = file;
    m_tooltip = tooltip;
    m_mode = mode;
    init();
    initConnections();
}

void CBrowseFileWidget::setPath(const QString &path)
{
    m_filePath = path;
    if(m_pEditFile)
        m_pEditFile->setText(path);
}

void CBrowseFileWidget::setFilter(const QString &filter)
{
    m_fileFilter = filter;
}

void CBrowseFileWidget::setMode(QFileDialog::FileMode mode)
{
    m_mode = mode;
}

QString CBrowseFileWidget::getPath() const
{
    return m_filePath;
}

void CBrowseFileWidget::clear()
{
    m_pEditFile->clear();
}

void CBrowseFileWidget::init()
{
    m_pEditFile = new QLineEdit(m_filePath);
    m_pBrowseBtn = new QPushButton("...");
    m_pBrowseBtn->setToolTip(m_tooltip);

    auto pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->addWidget(m_pEditFile);
    pLayout->addWidget(m_pBrowseBtn);
    setLayout(pLayout);
}

void CBrowseFileWidget::initConnections()
{
    connect(m_pBrowseBtn, &QPushButton::clicked, [&]
    {
        m_filePath = showSelectFileDlg();
        m_pEditFile->setText(m_filePath);
        emit selectedFileChanged(m_filePath);
    });
}

QString CBrowseFileWidget::showSelectFileDlg()
{
    QString filePath(m_filePath);
    QFileDialog fileDlg(this);
    fileDlg.setFileMode(m_mode);
    fileDlg.setViewMode(QFileDialog::Detail);
    fileDlg.setNameFilter(m_fileFilter);

    if(m_mode == QFileDialog::Directory)
        fileDlg.setOption(QFileDialog::ShowDirsOnly, true);

    if(fileDlg.exec())
    {
        QStringList pathList = fileDlg.selectedFiles();
        filePath = pathList.first();
    }
    return filePath;
}
