#ifndef COCVWIDGETDIVIDE_HPP
#define COCVWIDGETDIVIDE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvDivide.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetDivide : public CProtocolTaskWidget
{
    public:

        COcvWidgetDivide(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDivide(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDivideParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDivideParam>();

            auto pSpinScale = addDoubleSpin(0, tr("Scale"), m_pParam->m_scale, 0, DBL_MAX, 1, 4);
            auto pComboDepth = addCombo(1, tr("Depth"));
            pComboDepth->addItem("Default", -1);
            pComboDepth->addItem("CV_8U", CV_8U);
            pComboDepth->addItem("CV_16U", CV_16U);
            pComboDepth->addItem("CV_32F", CV_32F);
            pComboDepth->addItem("CV_64F", CV_64F);
            pComboDepth->setCurrentIndex(pComboDepth->findData(m_pParam->m_dtype));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_scale = pSpinScale->value();
                m_pParam->m_dtype = pComboDepth->currentData().toInt();
                emit doApplyProcess(m_pParam);
            } );

            
            
        }

    private:

        std::shared_ptr<COcvDivideParam> m_pParam = nullptr;
};

class COcvWidgetDivideFactory : public CWidgetFactory
{
    public:

        COcvWidgetDivideFactory()
        {
            m_name = QObject::tr("Divide").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDivide>(pParam);
        }
};
#endif // COCVWIDGETDIVIDE_HPP
