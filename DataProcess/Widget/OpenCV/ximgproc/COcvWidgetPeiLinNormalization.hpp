#ifndef COCVWIDGETPEILINNORMALIZATION_HPP
#define COCVWIDGETPEILINNORMALIZATION_HPP

#include "Process/OpenCV/ximgproc/COcvPeiLinNormalization.hpp"

class COcvWidgetPeiLinNormalization : public CProtocolTaskWidget
{
    public:

        COcvWidgetPeiLinNormalization(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetPeiLinNormalization(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvPeiLinNormalizationParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvPeiLinNormalizationParam>();

            auto pLabelInterpolation = new QLabel(tr("Interpolation"));
            auto pComboInterpolation = new QComboBox;
            pComboInterpolation->addItem(tr("Nearest neighbor"), cv::INTER_NEAREST);
            pComboInterpolation->addItem(tr("Bilinear"), cv::INTER_LINEAR);
            pComboInterpolation->addItem(tr("Cubic"), cv::INTER_CUBIC);
            pComboInterpolation->addItem(tr("Area"), cv::INTER_AREA);
            pComboInterpolation->addItem(tr("Lanczos 4"), cv::INTER_LANCZOS4);
            pComboInterpolation->addItem(tr("Exact bilinear"), cv::INTER_LINEAR_EXACT);
            pComboInterpolation->setCurrentIndex(pComboInterpolation->findData(m_pParam->m_interpolation));

            
            m_pLayout->addWidget(pLabelInterpolation, 0, 0);
            m_pLayout->addWidget(pComboInterpolation, 0, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_interpolation = pComboInterpolation->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvPeiLinNormalizationParam>  m_pParam = nullptr;
};

class COcvWidgetPeiLinNormalizationFactory : public CWidgetFactory
{
    public:

        COcvWidgetPeiLinNormalizationFactory()
        {
            m_name = QObject::tr("Pei-Lin Normalization").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetPeiLinNormalization>(pParam);
        }
};

#endif // COCVWIDGETPEILINNORMALIZATION_HPP
