#ifndef COCVWIDGETTRACKERKCF_HPP
#define COCVWIDGETTRACKERKCF_HPP
#include "Process/OpenCV/tracking/COcvTrackerKCF.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetTrackerKCF : public CWidgetApplyOnly
{
    public:

        COcvWidgetTrackerKCF(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetTrackerKCF(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }

    private:

        std::shared_ptr<COcvTrackerKCFParam> m_pParam = nullptr;
};

class COcvWidgetTrackerKCFFactory : public CWidgetFactory
{
    public:

        COcvWidgetTrackerKCFFactory()
        {
            m_name = QObject::tr("TrackerKCF").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetTrackerKCF>(pParam);
        }
};
#endif // COCVWIDGETTRACKERKCF_HPP
