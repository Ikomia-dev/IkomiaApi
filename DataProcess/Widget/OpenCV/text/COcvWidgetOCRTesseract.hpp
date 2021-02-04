#ifndef COCVWIDGETOCRTESSERACT_HPP
#define COCVWIDGETOCRTESSERACT_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/text/COcvOCRTesseract.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetOCRTesseract : public CWidgetApplyOnly
{
    public:

        COcvWidgetOCRTesseract(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetOCRTesseract(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }

    private:

        std::shared_ptr<COcvOCRTesseractParam> m_pParam = nullptr;
};

class COcvWidgetOCRTesseractFactory : public CWidgetFactory
{
    public:

        COcvWidgetOCRTesseractFactory()
        {
            m_name = QObject::tr("OCRTesseract").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetOCRTesseract>(pParam);
        }
};
#endif // COCVWIDGETOCRTESSERACT_HPP
