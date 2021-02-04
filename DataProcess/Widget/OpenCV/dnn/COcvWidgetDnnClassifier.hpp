#ifndef COCVWIDGETDNNCLASSIFIER_HPP
#define COCVWIDGETDNNCLASSIFIER_HPP

#include "COcvWidgetDnnProcess.h"
#include "Process/OpenCV/dnn/COcvDnnClassifier.hpp"

class COcvWidgetDnnClassifier: public COcvWidgetDnnProcess
{
    public:

        COcvWidgetDnnClassifier(QWidget *parent = Q_NULLPTR): COcvWidgetDnnProcess(parent)
        {
            init();
        }
        COcvWidgetDnnClassifier(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : COcvWidgetDnnProcess(pParam, parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDnnClassifierParam>(pParam);
            init();
        }

        void init() override
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDnnClassifierParam>();

            auto pParam = std::dynamic_pointer_cast<COcvDnnClassifierParam>(m_pParam);
            assert(pParam);

            m_pComboNetType = addCombo(tr("Network type"));
            m_pComboNetType->addItem("AlexNet", COcvDnnClassifierParam::NetworkType::ALEXNET);
            m_pComboNetType->addItem("GoogleNet", COcvDnnClassifierParam::NetworkType::GOOGLENET);
            m_pComboNetType->addItem("Inception", COcvDnnClassifierParam::NetworkType::INCEPTION);
            m_pComboNetType->addItem("VGG", COcvDnnClassifierParam::NetworkType::VGG);
            m_pComboNetType->addItem("ResNet", COcvDnnClassifierParam::NetworkType::RESNET);
            m_pComboNetType->setCurrentIndex(m_pComboNetType->findData(pParam->m_netType));

            initConnections();
        }

    private:

        void    initConnections()
        {
            connect(m_pComboNetType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &COcvWidgetDnnClassifier::onNetworkTypeChanged);
        }

    private slots:

        void    onNetworkTypeChanged(int index)
        {
            Q_UNUSED(index);
            auto pParam = std::dynamic_pointer_cast<COcvDnnClassifierParam>(m_pParam);
            assert(pParam);
            pParam->m_netType = m_pComboNetType->currentData().toInt();
        }

    private:

        QComboBox*  m_pComboNetType = nullptr;
};

class COcvWidgetDnnClassifierFactory : public CWidgetFactory
{
    public:

        COcvWidgetDnnClassifierFactory()
        {
            m_name = QObject::tr("DNN classification").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDnnClassifier>(pParam);
        }
};

#endif // COCVWIDGETDNNCLASSIFIER_HPP
