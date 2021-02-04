#include "CColorPushButton.h"
#include <QApplication>
#include <QColorDialog>

CColorPushButton::CColorPushButton(const QString& title, const QColor& color, QWidget *parent) : QPushButton(parent)
{
    m_title = title;
    m_color = color;

    init();
    initConnections();
}

QColor CColorPushButton::getColor() const
{
    return m_color;
}

void CColorPushButton::setColor(const QColor &color)
{
    m_color = color;
    updateColor();
}

void CColorPushButton::init()
{
    auto pal = qApp->palette();
    auto hoverBorderColor = pal.light().color();

    setFlat(true);
    setToolTip(m_title);
    setStyleSheet(QString("QPushButton { background-color: %1; border: none; } QPushButton:hover {border: 1px solid %2;}")
                  .arg(m_color.name())
                  .arg(hoverBorderColor.name()));
}

void CColorPushButton::initConnections()
{
    connect(this, &CColorPushButton::clicked, [&]
    {
        QColor newColor = QColorDialog::getColor(m_color, nullptr, tr("Color choice"));
        if(newColor.isValid() && newColor != m_color)
        {
            m_color = newColor;
            updateColor();
            emit colorChanged(m_color);
        }
    });
}

void CColorPushButton::updateColor()
{
    auto pal = qApp->palette();
    auto hoverBorderColor = pal.light().color();
    setStyleSheet(QString("QPushButton { background-color: %1; border: none; } QPushButton:hover {border: 1px solid %2;}")
                  .arg(m_color.name())
                  .arg(hoverBorderColor.name()));
}

#include "moc_CColorPushButton.cpp"
