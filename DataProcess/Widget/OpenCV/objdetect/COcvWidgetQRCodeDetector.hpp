#ifndef COCVWIDGETQRCODEDETECTOR_HPP
#define COCVWIDGETQRCODEDETECTOR_HPP
#include "Process/OpenCV/objdetect/COcvQRCodeDetector.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetQRCodeDetector : public CProtocolTaskWidget
{
    public:

        COcvWidgetQRCodeDetector(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetQRCodeDetector(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvQRCodeDetectorParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvQRCodeDetectorParam>();

            auto pSpinEpsX = addDoubleSpin(0, tr("Eps x"), m_pParam->m_eps_x, 0, DBL_MAX, 0.1);
            auto pSpinEpsY = addDoubleSpin(1, tr("Eps y"), m_pParam->m_eps_y, 0, DBL_MAX, 0.1);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_eps_x = pSpinEpsX->value();
                m_pParam->m_eps_y = pSpinEpsY->value();
                emit doApplyProcess(m_pParam); } );

            
            
        }

    private:

        std::shared_ptr<COcvQRCodeDetectorParam> m_pParam = nullptr;
};

class COcvWidgetQRCodeDetectorFactory : public CWidgetFactory
{
    public:

        COcvWidgetQRCodeDetectorFactory()
        {
            m_name = QObject::tr("QRCodeDetector").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetQRCodeDetector>(pParam);
        }
};
#endif // COCVWIDGETQRCODEDETECTOR_HPP
