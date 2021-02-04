#include "CProtocolTaskWidgetWrap.h"
#include <QLayout>
#include <QDebug>

CProtocolTaskWidgetWrap::CProtocolTaskWidgetWrap() : CProtocolTaskWidget(nullptr)
{
}

CProtocolTaskWidgetWrap::CProtocolTaskWidgetWrap(QWidget *parent) : CProtocolTaskWidget(parent)
{
}

void CProtocolTaskWidgetWrap::init()
{
    CPyEnsureGIL gil;
    try
    {
        this->get_override("init")();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::onApply()
{
    CPyEnsureGIL gil;
    try
    {
        if(override onApplyOver = this->get_override("onApply"))
            onApplyOver();
        else
            CProtocolTaskWidget::onApply();
    }
    catch(boost::python::error_already_set&)
    {
        //Do not throw exceptions from slot
        qCritical().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CProtocolTaskWidgetWrap::default_onApply()
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTaskWidget::onApply();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::setLayout(long long layoutPtr)
{
    CPyEnsureGIL gil;
    try
    {
        auto pLayout = reinterpret_cast<QLayout*>(layoutPtr);
        m_pLayout->addLayout(pLayout, 0, 0);
        //this->CProtocolTaskWidget::setLayout(pLayout);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::emitApply(const ProtocolTaskParamPtr& paramPtr)
{
    try
    {
        emit doApplyProcess(paramPtr);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::emitSendProcessAction(int flags)
{
    try
    {
        emit doSendProcessAction(flags);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::emitSetGraphicsTool(GraphicsShape tool)
{
    try
    {
        emit doSetGraphicsTool(tool);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskWidgetWrap::emitSetGraphicsCategory(const std::string &category)
{
    try
    {
        emit doSetGraphicsCategory(QString::fromStdString(category));
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

