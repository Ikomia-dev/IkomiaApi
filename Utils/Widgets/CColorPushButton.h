#ifndef CCOLORPUSHBUTTON_H
#define CCOLORPUSHBUTTON_H

#include <QPushButton>
#include "UtilsGlobal.hpp"

class UTILSSHARED_EXPORT CColorPushButton : public QPushButton
{
    Q_OBJECT

    public:

        CColorPushButton(const QString &title, const QColor& color, QWidget* parent = nullptr);

        QColor  getColor() const;

        void    setColor(const QColor& color);

    signals:

        void    colorChanged(const QColor& color);

    private:

        void    init();
        void    initConnections();

        void    updateColor();

    private:

        QColor  m_color = Qt::red;
        QString m_title;
};

#endif // CCOLORPUSHBUTTON_H
