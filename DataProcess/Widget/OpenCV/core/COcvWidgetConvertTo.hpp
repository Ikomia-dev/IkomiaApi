#ifndef COCVWIDGETCONVERTTO_HPP
#define COCVWIDGETCONVERTTO_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvConvertTo.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetConvertTo : public CProtocolTaskWidget
{
    public:

        COcvWidgetConvertTo(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetConvertTo(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvConvertToParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvConvertToParam>();

            auto pSpinAlpha = addDoubleSpin(0, tr("Alpha"), m_pParam->m_alpha);
            auto pSpinBeta = addDoubleSpin(1, tr("Beta"), m_pParam->m_beta);

            auto pComboDepth = addCombo(2, tr("Depth"));
            pComboDepth->addItem("Default", -1);
            pComboDepth->addItem("CV_8U", CV_8U);
            pComboDepth->addItem("CV_8S", CV_8S);
            pComboDepth->addItem("CV_16U", CV_16U);
            pComboDepth->addItem("CV_16S", CV_16S);
            pComboDepth->addItem("CV_32S", CV_32S);
            pComboDepth->addItem("CV_32F", CV_32F);
            pComboDepth->addItem("CV_64F", CV_64F);
            pComboDepth->setCurrentIndex(pComboDepth->findData(m_pParam->m_dtype));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_dtype = pComboDepth->currentData().toInt();
                m_pParam->m_alpha = pSpinAlpha->value();
                m_pParam->m_beta = pSpinBeta->value();
                emit doApplyProcess(m_pParam);
            } );

            
        }

    private:

        std::shared_ptr<COcvConvertToParam> m_pParam = nullptr;
};

class COcvWidgetConvertToFactory : public CWidgetFactory
{
    public:

        COcvWidgetConvertToFactory()
        {
            m_name = QObject::tr("ConvertTo").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetConvertTo>(pParam);
        }
};
#endif // COCVWIDGETCONVERTTO_HPP
