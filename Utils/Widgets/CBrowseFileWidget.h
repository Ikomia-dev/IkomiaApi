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
