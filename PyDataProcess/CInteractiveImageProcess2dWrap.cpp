#include "CInteractiveImageProcess2dWrap.h"

//---------------------------------------------------------//
//-----------  CInteractiveImageProcess2dWrap  ------------//
//---------------------------------------------------------//
CInteractiveImageProcess2dWrap::CInteractiveImageProcess2dWrap() : CInteractiveImageProcess2d()
{
}

CInteractiveImageProcess2dWrap::CInteractiveImageProcess2dWrap(const std::string &name) : CInteractiveImageProcess2d(name)
{
}

CInteractiveImageProcess2dWrap::CInteractiveImageProcess2dWrap(const CInteractiveImageProcess2d &process) : CInteractiveImageProcess2d(process)
{
}

size_t CInteractiveImageProcess2dWrap::getProgressSteps()
{
    CPyEnsureGIL gil;
    try
    {
        if(override getProgressStepsOver = this->get_override("getProgressSteps"))
            return getProgressStepsOver();

        return CInteractiveImageProcess2d::getProgressSteps();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CInteractiveImageProcess2dWrap::default_getProgressSteps()
{
    CPyEnsureGIL gil;
    try
    {
        return this->CInteractiveImageProcess2d::getProgressSteps();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CInteractiveImageProcess2dWrap::getProgressSteps(size_t unitEltCount)
{
    CPyEnsureGIL gil;
    try
    {
        if(override getProgressStepsOver = this->get_override("getProgressSteps"))
            return getProgressStepsOver(unitEltCount);

        return CInteractiveImageProcess2d::getProgressSteps(unitEltCount);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

size_t CInteractiveImageProcess2dWrap::default_getProgressSteps(size_t unitEltCount)
{
    CPyEnsureGIL gil;
    try
    {
        return this->CInteractiveImageProcess2d::getProgressSteps(unitEltCount);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::setActive(bool bActive)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setActiveOver = this->get_override("setActive"))
            setActiveOver(bActive);
        else
            CInteractiveImageProcess2d::setActive(bActive);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::default_setActive(bool bActive)
{
    CPyEnsureGIL gil;
    try
    {
        this->CInteractiveImageProcess2d::setActive(bActive);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::updateStaticOutputs()
{
    CPyEnsureGIL gil;
    try
    {
        if(override updateStaticOutputsOver = this->get_override("updateStaticOutputs"))
            updateStaticOutputsOver();
        else
            CInteractiveImageProcess2d::updateStaticOutputs();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::default_updateStaticOutputs()
{
    CPyEnsureGIL gil;
    try
    {
        this->CInteractiveImageProcess2d::updateStaticOutputs();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::beginTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        if(override beginTaskRunOver = this->get_override("beginTaskRun"))
            beginTaskRunOver();
        else
            CInteractiveImageProcess2d::beginTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::default_beginTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        this->CInteractiveImageProcess2d::beginTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::endTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        if(override endTaskRunOver = this->get_override("endTaskRun"))
            endTaskRunOver();
        else
            CInteractiveImageProcess2d::endTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::default_endTaskRun()
{
    CPyEnsureGIL gil;
    try
    {
        this->CInteractiveImageProcess2d::endTaskRun();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::run()
{
    CPyEnsureGIL gil;
    try
    {
        if(override runOver = this->get_override("run"))
            runOver();
        else
            CInteractiveImageProcess2d::run();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::default_run()
{
    CPyEnsureGIL gil;
    try
    {
        this->CInteractiveImageProcess2d::run();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::stop()
{
    CPyEnsureGIL gil;
    try
    {
        if(override stopOver = this->get_override("stop"))
            stopOver();
        else
            CInteractiveImageProcess2d::stop();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::default_stop()
{
    CPyEnsureGIL gil;
    try
    {
        this->CInteractiveImageProcess2d::stop();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::graphicsChanged()
{
    CPyEnsureGIL gil;
    try
    {
        if(override graphicsChangedOver = this->get_override("graphicsChanged"))
            graphicsChangedOver();
        else
            CInteractiveImageProcess2d::graphicsChanged();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::default_graphicsChanged()
{
    CPyEnsureGIL gil;
    try
    {
        this->CInteractiveImageProcess2d::graphicsChanged();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::globalInputChanged(bool bNewSequence)
{
    CPyEnsureGIL gil;
    try
    {
        if(override globalInputChangedOver = this->get_override("globalInputChanged"))
            globalInputChangedOver(bNewSequence);
        else
            CInteractiveImageProcess2d::globalInputChanged(bNewSequence);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::default_globalInputChanged(bool bNewSequence)
{
    CPyEnsureGIL gil;
    try
    {
        this->CInteractiveImageProcess2d::globalInputChanged(bNewSequence);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CInteractiveImageProcess2dWrap::emitGraphicsContextChanged()
{
    emit m_signalHandler->doGraphicsContextChanged();
}

void CInteractiveImageProcess2dWrap::emitStepProgress()
{
    emit m_signalHandler->doProgress();
}

void CInteractiveImageProcess2dWrap::emitOutputChanged()
{
    emit m_signalHandler->doOutputChanged();
}


