#ifndef COCVWIDGETEQUALIZEHIST_HPP
#define COCVWIDGETEQUALIZEHIST_HPP

#include "Process/OpenCV/imgproc/COcvEqualizeHist.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetEqualizeHist : public CWidgetApplyOnly
{
    public:

        COcvWidgetEqualizeHist(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetEqualizeHist(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetEqualizeHistFactory : public CWidgetFactory
{
    public:

        COcvWidgetEqualizeHistFactory()
        {
            m_name = QObject::tr("Equalize histogram").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetEqualizeHist>(pParam);
        }
};

#endif // COCVWIDGETEQUALIZEHIST_HPP
