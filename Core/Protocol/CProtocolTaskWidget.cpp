#include "CProtocolTaskWidget.h"

CProtocolTaskWidget::CProtocolTaskWidget(QWidget *parent) : QWidget(parent)
{
    m_pLayout = new QGridLayout;
    m_pApplyBtn = new QPushButton(tr("Apply"));
    m_pApplyBtn->setObjectName("CProcessBtn");
    m_pApplyBtn->setDefault(true);

    connect(m_pApplyBtn, &QPushButton::clicked, this, &CProtocolTaskWidget::onApply);

    // Create layout to keep gridlayout widgets close to each other
    QVBoxLayout* pContainerLayout = new QVBoxLayout;
    pContainerLayout->addLayout(m_pLayout);
    pContainerLayout->addStretch(0);

    // Create widget containing gridlayout
    QWidget* pWidget = new QWidget;
    pWidget->setObjectName("CWidget");
    pWidget->setLayout(pContainerLayout);

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
}

void CProtocolTaskWidget::setApplyBtnHidden(bool bHidden)
{
    m_pApplyBtn->setHidden(bHidden);
}

void CProtocolTaskWidget::onApply()
{
    return;
}

QComboBox *CProtocolTaskWidget::addCombo(int row, const QString &title)
{
    auto pLabel = new QLabel(title);
    auto pCombo = new QComboBox;
    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pCombo, row, 1);
    return pCombo;
}

QComboBox *CProtocolTaskWidget::addCombo(const QString &title)
{
    int nextPos = m_pLayout->rowCount();
    return addCombo(nextPos, title);
}

QSpinBox *CProtocolTaskWidget::addSpin(int row, const QString &title, int value, int min, int max, int step)
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

QSpinBox *CProtocolTaskWidget::addSpin(const QString &title, int value, int min, int max, int step)
{
    int nextPos = m_pLayout->rowCount();
    return addSpin(nextPos, title, value, min, max, step);
}

QDoubleSpinBox *CProtocolTaskWidget::addDoubleSpin(int row, const QString &title, double value, double min, double max, double step, int decimals)
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

QDoubleSpinBox *CProtocolTaskWidget::addDoubleSpin(const QString &title, double value, double min, double max, double step, int decimals)
{
    int nextPos = m_pLayout->rowCount();
    return addDoubleSpin(nextPos, title, value, min, max, step, decimals);
}

QCheckBox *CProtocolTaskWidget::addCheck(int row, const QString &title, bool isChecked)
{
    QCheckBox* pCheck = new QCheckBox(title);
    pCheck->setChecked(isChecked);
    m_pLayout->addWidget(pCheck, row, 0, 1, 2);

    return pCheck;
}

QCheckBox *CProtocolTaskWidget::addCheck(const QString &title, bool isChecked)
{
    int nextPos = m_pLayout->rowCount();
    return addCheck(nextPos, title, isChecked);
}

QRadioButton *CProtocolTaskWidget::addRadio(int row, const QString &title, bool isChecked)
{
    QRadioButton* pRadio = new QRadioButton(title);
    pRadio->setChecked(isChecked);
    m_pLayout->addWidget(pRadio, row, 0, 1, 2);

    return pRadio;
}

QRadioButton *CProtocolTaskWidget::addRadio(const QString &title, bool isChecked)
{
    int nextPos = m_pLayout->rowCount();
    return addRadio(nextPos, title, isChecked);
}

QSlider *CProtocolTaskWidget::addSlider(int row, const QString &title, int value, int min, int max, int step)
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

QSlider *CProtocolTaskWidget::addSlider(const QString &title, int value, int min, int max, int step)
{
    int nextPos = m_pLayout->rowCount();
    return addSlider(nextPos, title, value, min, max, step);
}

CColorPushButton *CProtocolTaskWidget::addColorButton(int row, const QString &title, const QColor &color)
{
    auto pLabel = new QLabel(title);
    auto pBtn = new CColorPushButton(title, color);
    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pBtn, row, 1);
    return pBtn;
}

CColorPushButton *CProtocolTaskWidget::addColorButton(const QString &title, const QColor &color)
{
    int nextPos = m_pLayout->rowCount();
    return addColorButton(nextPos, title, color);
}

CBrowseFileWidget *CProtocolTaskWidget::addBrowseFile(int row, const QString &title, const QString &path, const QString &tooltip)
{
    auto pLabel = new QLabel(title);
    auto pBrowseWidget = new CBrowseFileWidget(path, tooltip);
    m_pLayout->addWidget(pLabel, row, 0);
    m_pLayout->addWidget(pBrowseWidget, row, 1);
    return pBrowseWidget;
}

CBrowseFileWidget *CProtocolTaskWidget::addBrowseFile(const QString &title, const QString &path, const QString &tooltip)
{
    int nextPos = m_pLayout->rowCount();
    return addBrowseFile(nextPos, title, path, tooltip);
}

#include "moc_CProtocolTaskWidget.cpp"
