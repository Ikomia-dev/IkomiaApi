#include "CVideoProcess.h"

CVideoProcess::CVideoProcess() : CImageProcess2d()
{
    m_type = CProtocolTask::Type::VIDEO;
}

CVideoProcess::CVideoProcess(const std::string& name) : CImageProcess2d(name)
{
    m_type = CProtocolTask::Type::VIDEO;
}

void CVideoProcess::beginTaskRun()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_bRunning = true;

    //Notify current task name
    emit m_signalHandler->doSetMessage(QString::fromStdString(m_name));

    //Check inputs validity
    if(getInputCount() == 0)
        throw CException(CoreExCode::INVALID_PARAMETER, "No input", __func__, __FILE__, __LINE__);

    //Start timing
    m_timer.start();

    // Clear graphics as in CImageProcess2d::beginTaskRun
    m_graphicsMasks.clear();
}

void CVideoProcess::notifyVideoStart(int frameCount)
{
    Q_UNUSED(frameCount);
}

void CVideoProcess::notifyVideoEnd()
{
}
