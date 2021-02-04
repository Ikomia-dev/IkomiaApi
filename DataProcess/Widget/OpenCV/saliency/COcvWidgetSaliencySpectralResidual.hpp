#ifndef COCVWIDGETSALIENCYSPECTRALRESIDUAL_HPP
#define COCVWIDGETSALIENCYSPECTRALRESIDUAL_HPP
#include "Process/OpenCV/saliency/COcvSaliencySpectralResidual.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetSaliencySpectralResidual : public CWidgetApplyOnly
{
    public:

        COcvWidgetSaliencySpectralResidual(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetSaliencySpectralResidual(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetSaliencySpectralResidualFactory : public CWidgetFactory
{
    public:

        COcvWidgetSaliencySpectralResidualFactory()
        {
            m_name = QObject::tr("StaticSaliencySpectralResidual").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSaliencySpectralResidual>(pParam);
        }
};
#endif // COCVWIDGETSALIENCYSPECTRALRESIDUAL_HPP
