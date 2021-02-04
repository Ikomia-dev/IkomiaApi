#ifndef COCVWIDGETRETINA_HPP
#define COCVWIDGETRETINA_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bioinspired/COcvRetina.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetRetina : public CProtocolTaskWidget
{
    public:

        COcvWidgetRetina(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetRetina(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvRetinaParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvRetinaParam>();

            auto pCheck = addCheck(0, tr("Use OpenCL"), m_pParam->m_bUseOCL);
            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_bUseOCL = pCheck->isChecked();
                emit doApplyProcess(m_pParam);
            } );

            
        }

    private:

        std::shared_ptr<COcvRetinaParam> m_pParam = nullptr;
};

class COcvWidgetRetinaFactory : public CWidgetFactory
{
    public:

        COcvWidgetRetinaFactory()
        {
            m_name = QObject::tr("Retina").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetRetina>(pParam);
        }
};
#endif // COCVWIDGETRETINA_HPP
