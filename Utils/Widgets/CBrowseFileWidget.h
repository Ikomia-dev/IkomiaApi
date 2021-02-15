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

#ifndef CBROWSEFILEWIDGET_H
#define CBROWSEFILEWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include "UtilsGlobal.hpp"

class UTILSSHARED_EXPORT CBrowseFileWidget : public QWidget
{
    Q_OBJECT

    public:

        CBrowseFileWidget(QWidget* pParent = Q_NULLPTR);
        CBrowseFileWidget(const QString &file, const QString &tooltip, QFileDialog::FileMode mode=QFileDialog::ExistingFile, QWidget* pParent = Q_NULLPTR);

        void    setPath(const QString& path);
        void    setFilter(const QString& filter);
        void    setMode(QFileDialog::FileMode mode);

        QString getPath() const;

        void    clear();

    signals:

        void    selectedFileChanged(const QString& filePath);

    private:

        void    init();
        void    initConnections();

        QString showSelectFileDlg();

    private:

        QString                 m_filePath;
        QString                 m_fileFilter;
        QString                 m_tooltip;
        QFileDialog::FileMode   m_mode = QFileDialog::ExistingFile;
        QLineEdit*              m_pEditFile = nullptr;
        QPushButton*            m_pBrowseBtn = nullptr;
};

#endif // CBROWSEFILEWIDGET_H
