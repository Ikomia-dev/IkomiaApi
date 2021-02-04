#ifndef COCVWIDGETSALIENCYFINEGRAINED_HPP
#define COCVWIDGETSALIENCYFINEGRAINED_HPP
#include "Process/OpenCV/saliency/COcvSaliencyFineGrained.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetSaliencyFineGrained : public CWidgetApplyOnly
{
    public:

        COcvWidgetSaliencyFineGrained(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetSaliencyFineGrained(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetSaliencyFineGrainedFactory : public CWidgetFactory
{
    public:

        COcvWidgetSaliencyFineGrainedFactory()
        {
            m_name = QObject::tr("StaticSaliencyFineGrained").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSaliencyFineGrained>(pParam);
        }
};
#endif // COCVWIDGETSALIENCYFINEGRAINED_HPP
