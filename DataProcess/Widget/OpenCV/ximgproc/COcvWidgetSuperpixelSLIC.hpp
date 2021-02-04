#ifndef COCVWIDGETSUPERPIXELSLIC_HPP
#define COCVWIDGETSUPERPIXELSLIC_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvSuperpixelSLIC.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSuperpixelSLIC : public CProtocolTaskWidget
{
    public:

        COcvWidgetSuperpixelSLIC(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSuperpixelSLIC(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSuperpixelSLICParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSuperpixelSLICParam>();

            auto pCombo = addCombo(0, tr("Algorithm"));
            pCombo->addItem("SLIC", cv::ximgproc::SLIC);
            pCombo->addItem("SLICO", cv::ximgproc::SLICO);
            pCombo->addItem("MSLIC", cv::ximgproc::MSLIC);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_algorithm));
            auto pSpinSize = addSpin(1, tr("Region size"), m_pParam->m_region_size);
            auto pSpinRuler = addDoubleSpin(2, tr("Ruler"), m_pParam->m_ruler);
            auto pSpinIter = addSpin(3, tr("Iterations"), m_pParam->m_num_iterations);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_region_size = pSpinSize->value();
                m_pParam->m_ruler = pSpinRuler->value();
                m_pParam->m_num_iterations = pSpinIter->value();
                m_pParam->m_algorithm = pCombo->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvSuperpixelSLICParam> m_pParam = nullptr;
};

class COcvWidgetSuperpixelSLICFactory : public CWidgetFactory
{
    public:

        COcvWidgetSuperpixelSLICFactory()
        {
            m_name = QObject::tr("SuperpixelSLIC").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSuperpixelSLIC>(pParam);
        }
};
#endif // COCVWIDGETSUPERPIXELSLIC_HPP
