#ifndef CPROGRESSSIGNALHANDLER_H
#define CPROGRESSSIGNALHANDLER_H

#include <QObject>
#include "UtilsGlobal.hpp"

/// @cond INTERNAL

class UTILSSHARED_EXPORT CProgressSignalHandler : public QObject
{
    Q_OBJECT

    public:

        CProgressSignalHandler();

        void    emitSetTotalSteps(int maxSteps);
        void    emitSetMessage(const std::string& msg);
        void    emitSetElapsedTime(double time);
        void    emitProgress();

    signals:

        void    doSetTotalSteps(int maxSteps);
        void    doSetMessage(const QString& msg);
        void    doSetElapsedTime(double elapsedTime);
        void    doSetValue(int value);
        void    doFinish();
        void    doProgress();

    public slots:

        void    onSetTotalSteps(int maxSteps);
        void    onSetMessage(const QString& msg);
        void    onSetElapsedTime(double elapsedTime);
        void    onProgress();
};

/// @endcond

#endif // CPROGRESSSIGNALHANDLER_H
