#ifndef COCVWIDGETSEAMLESSCLONING_HPP
#define COCVWIDGETSEAMLESSCLONING_HPP
#include "Process/OpenCV/photo/COcvSeamlessCloning.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetSeamlessCloning : public CProtocolTaskWidget
{
    public:

        COcvWidgetSeamlessCloning(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSeamlessCloning(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSeamlessCloningParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSeamlessCloningParam>();

            auto pCombo = addCombo(0, tr("Cloning method"));
            pCombo->addItem(tr("NORMAL_CLONE"), cv::NORMAL_CLONE);
            pCombo->addItem(tr("MIXED_CLONE"), cv::MIXED_CLONE);
            pCombo->addItem(tr("MONOCHROME_TRANSFER"), cv::MONOCHROME_TRANSFER);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_flags));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_flags = pCombo->currentData().toInt();
                emit doApplyProcess(m_pParam); } );

            
        }

    private:

        std::shared_ptr<COcvSeamlessCloningParam> m_pParam = nullptr;
};

class COcvWidgetSeamlessCloningFactory : public CWidgetFactory
{
    public:

        COcvWidgetSeamlessCloningFactory()
        {
            m_name = QObject::tr("SeamlessCloning").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSeamlessCloning>(pParam);
        }
};
#endif // COCVWIDGETSEAMLESSCLONING_HPP
