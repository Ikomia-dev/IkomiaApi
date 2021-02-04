#include "CProgressSignalHandler.h"

CProgressSignalHandler::CProgressSignalHandler()
{
}

void CProgressSignalHandler::emitSetTotalSteps(int maxSteps)
{
    emit doSetTotalSteps(maxSteps);
}

void CProgressSignalHandler::emitSetMessage(const std::string &msg)
{
    emit doSetMessage(QString::fromStdString(msg));
}

void CProgressSignalHandler::emitSetElapsedTime(double time)
{
    emit doSetElapsedTime(time);
}

void CProgressSignalHandler::emitProgress()
{
    emit doProgress();
}

void CProgressSignalHandler::onSetTotalSteps(int maxSteps)
{
    emit doSetTotalSteps(maxSteps);
}

void CProgressSignalHandler::onSetMessage(const QString &msg)
{
    emit doSetMessage(msg);
}

void CProgressSignalHandler::onSetElapsedTime(double elapsedTime)
{
    emit doSetElapsedTime(elapsedTime);
}

void CProgressSignalHandler::onProgress()
{
    emit doProgress();
}
