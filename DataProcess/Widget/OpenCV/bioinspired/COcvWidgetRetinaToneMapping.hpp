#ifndef COCVWIDGETRETINATONEMAPPING_HPP
#define COCVWIDGETRETINATONEMAPPING_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bioinspired/COcvRetinaToneMapping.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetRetinaToneMapping : public CProtocolTaskWidget
{
    public:

        COcvWidgetRetinaToneMapping(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetRetinaToneMapping(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvRetinaToneMappingParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvRetinaToneMappingParam>();

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                emit doApplyProcess(m_pParam);
            } );

            
        }

    private:

        std::shared_ptr<COcvRetinaToneMappingParam> m_pParam = nullptr;
};

class COcvWidgetRetinaToneMappingFactory : public CWidgetFactory
{
    public:

        COcvWidgetRetinaToneMappingFactory()
        {
            m_name = QObject::tr("RetinaToneMapping").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetRetinaToneMapping>(pParam);
        }
};
#endif // COCVWIDGETRETINATONEMAPPING_HPP
