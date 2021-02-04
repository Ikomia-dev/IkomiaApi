#ifndef COCVWIDGETRETINASEGMENTATION_HPP
#define COCVWIDGETRETINASEGMENTATION_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bioinspired/COcvRetinaSegmentation.hpp"
#include <QDoubleSpinBox>
#include <QLabel>
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetRetinaSegmentation : public CWidgetApplyOnly
{
    public:

        COcvWidgetRetinaSegmentation(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetRetinaSegmentation(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }

    private:

        std::shared_ptr<COcvRetinaSegmentationParam> m_pParam = nullptr;
};

class COcvWidgetRetinaSegmentationFactory : public CWidgetFactory
{
    public:

        COcvWidgetRetinaSegmentationFactory()
        {
            m_name = QObject::tr("RetinaSegmentation").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetRetinaSegmentation>(pParam);
        }
};
#endif // COCVWIDGETRETINASEGMENTATION_HPP
