#ifndef COCVWIDGETDNNCOLORIZATION_HPP
#define COCVWIDGETDNNCOLORIZATION_HPP

#include "COcvWidgetDnnProcess.h"
#include "Process/OpenCV/dnn/COcvDnnColorization.hpp"

class COcvWidgetDnnColorization: public COcvWidgetDnnProcess
{
    public:

        COcvWidgetDnnColorization(QWidget *parent = Q_NULLPTR): COcvWidgetDnnProcess(parent)
        {
        }
        COcvWidgetDnnColorization(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : COcvWidgetDnnProcess(pParam, parent)
        {
        }
};

class COcvWidgetDnnColorizationFactory : public CWidgetFactory
{
    public:

        COcvWidgetDnnColorizationFactory()
        {
            m_name = QObject::tr("DNN colorization").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDnnColorization>(pParam);
        }
};

#endif // COCVWIDGETDNNCOLORIZATION_HPP
