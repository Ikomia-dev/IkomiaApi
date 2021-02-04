#ifndef COCVWIDGETDNNDETECTOR_HPP
#define COCVWIDGETDNNDETECTOR_HPP

#include "COcvWidgetDnnProcess.h"
#include "Process/OpenCV/dnn/COcvDnnDetector.hpp"

class COcvWidgetDnnDetector: public COcvWidgetDnnProcess
{
    public:

        COcvWidgetDnnDetector(QWidget *parent = Q_NULLPTR): COcvWidgetDnnProcess(parent)
        {
            init();
        }
        COcvWidgetDnnDetector(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : COcvWidgetDnnProcess(pParam, parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(pParam);
            init();
        }

        void init() override
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDnnDetectorParam>();

            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);
            assert(pParam);

            m_pComboNetType = addCombo(tr("Network type"));
            m_pComboNetType->addItem("Faster R-CNN", COcvDnnDetectorParam::NetworkType::FASTER_RCNN);
            m_pComboNetType->addItem("R-FCN", COcvDnnDetectorParam::NetworkType::R_FCN);
            m_pComboNetType->addItem("SSD", COcvDnnDetectorParam::NetworkType::SSD);
            m_pComboNetType->addItem("YOLO", COcvDnnDetectorParam::NetworkType::YOLO);
            m_pComboNetType->addItem("Face Detector", COcvDnnDetectorParam::NetworkType::FACE_DETECTOR);
            m_pComboNetType->addItem("EAST (Text detection)", COcvDnnDetectorParam::NetworkType::EAST);
            m_pComboNetType->setCurrentIndex(m_pComboNetType->findData(pParam->m_netType));

            m_pSpinConfidence = addDoubleSpin(tr("Confidence"), pParam->m_confidence, 0.0, 1.0, 0.1, 2);
            m_pSpinNmsThreshold = addDoubleSpin(tr("NMS threshold"), pParam->m_nmsThreshold, 0.0, 1.0, 0.1, 2);

            initConnections();
        }

    private:

        void    initConnections()
        {
            connect(m_pComboNetType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &COcvWidgetDnnDetector::onNetworkTypeChanged);
            connect(m_pSpinConfidence, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&](double val)
            {
                auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);
                assert(pParam);
                pParam->m_confidence = val;
            });
            connect(m_pSpinNmsThreshold, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&](double val)
            {
                auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);
                assert(pParam);
                pParam->m_nmsThreshold = val;
            });
        }

    private slots:

        void    onNetworkTypeChanged(int index)
        {
            Q_UNUSED(index);
            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);
            assert(pParam);
            pParam->m_netType = m_pComboNetType->currentData().toInt();
        }

    private:

        QComboBox*      m_pComboNetType = nullptr;
        QDoubleSpinBox* m_pSpinConfidence = nullptr;
        QDoubleSpinBox* m_pSpinNmsThreshold = nullptr;
};

class COcvWidgetDnnDetectorFactory : public CWidgetFactory
{
    public:

        COcvWidgetDnnDetectorFactory()
        {
            m_name = QObject::tr("DNN detection").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDnnDetector>(pParam);
        }
};

#endif // COCVWIDGETDNNDETECTOR_HPP
