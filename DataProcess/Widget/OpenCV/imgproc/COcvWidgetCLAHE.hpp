#ifndef COCVWIDGETCLAHE_HPP
#define COCVWIDGETCLAHE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvCLAHE.hpp"
#include <QSpinBox>

class COcvWidgetCLAHE : public CProtocolTaskWidget
{
    public:

        COcvWidgetCLAHE(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCLAHE(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCLAHEParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCLAHEParam>();

            auto pSpinClipLimit = addDoubleSpin(0, tr("Threshold for contrast limiting"), m_pParam->m_clipLimit);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_clipLimit = pSpinClipLimit->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvCLAHEParam> m_pParam = nullptr;
};

class COcvWidgetCLAHEFactory : public CWidgetFactory
{
    public:

        COcvWidgetCLAHEFactory()
        {
            m_name = QObject::tr("CLAHE").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetCLAHE>(pParam);
        }
};

#endif // COCVWIDGETCLAHE_HPP
