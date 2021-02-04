#ifndef COCVWIDGETSTRUCTUREDEDGEDETECTION_HPP
#define COCVWIDGETSTRUCTUREDEDGEDETECTION_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvStructuredEdgeDetection.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetStructuredEdgeDetection : public CWidgetApplyOnly
{
    public:

        COcvWidgetStructuredEdgeDetection(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetStructuredEdgeDetection(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }

    private:

        std::shared_ptr<COcvStructuredEdgeDetectionParam> m_pParam = nullptr;
};

class COcvWidgetStructuredEdgeDetectionFactory : public CWidgetFactory
{
    public:

        COcvWidgetStructuredEdgeDetectionFactory()
        {
            m_name = QObject::tr("StructuredEdgeDetection").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetStructuredEdgeDetection>(pParam);
        }
};
#endif // COCVWIDGETSTRUCTUREDEDGEDETECTION_HPP
