#ifndef COCVWIDGETMOTIONSALIENCYBINWANGAPR2014_HPP
#define COCVWIDGETMOTIONSALIENCYBINWANGAPR2014_HPP
#include "Process/OpenCV/saliency/COcvMotionSaliencyBinWangApr2014.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetMotionSaliencyBinWangApr2014 : public CWidgetApplyOnly
{
    public:

        COcvWidgetMotionSaliencyBinWangApr2014(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetMotionSaliencyBinWangApr2014(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetMotionSaliencyBinWangApr2014Factory : public CWidgetFactory
{
    public:

        COcvWidgetMotionSaliencyBinWangApr2014Factory()
        {
            m_name = QObject::tr("MotionSaliencyBinWangApr2014").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetMotionSaliencyBinWangApr2014>(pParam);
        }
};
#endif // COCVWIDGETMOTIONSALIENCYBINWANGAPR2014_HPP
