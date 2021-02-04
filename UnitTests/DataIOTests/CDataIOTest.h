#ifndef CDATAIOTEST_H
#define CDATAIOTEST_H

#include <QObject>

/**
 * @brief
 *
 */
class CDataIOTest : public QObject
{
    Q_OBJECT

    public:

        explicit CDataIOTest(QObject *parent = nullptr);

    private slots:

        void    readOpencvImage();
        void    readVideoFrame();

        void    playVideo();

        void    saveOpencvImage();

        void    writeVideo();

        void    testTools();
};

#endif // CDATAIOTEST_H
