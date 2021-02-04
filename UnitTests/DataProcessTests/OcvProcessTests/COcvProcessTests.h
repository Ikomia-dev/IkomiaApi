#ifndef COCVPROCESSTESTS_H
#define COCVPROCESSTESTS_H

#include <QObject>
#include "DataProcess.hpp"

class COcvProcessTests : public QObject
{
    Q_OBJECT

    public:

        COcvProcessTests(QObject *parent = nullptr);

    private slots:

        void    initTestCase();
        void    init();

        //bgsegm
        void    bckSubCnt();
        void    bckSubGmg();
        void    bckSubGsoc();
        void    bckSubLsbp();
        void    bckSubMog();

        //bioinspired
        void    retina();
        void    retinaSegmentation();

        //core
        void    dft();
        void    exp();
        void    extractChannel();
        void    flip();
        void    inRange();
        void    kmeans();
        void    log();
        void    negative();
        void    normalize();
        void    rotate();

        //features2d
        void    agast();
        void    akaze();
        void    brisk();
        void    fast();
        void    gftt();
        void    kaze();
        void    orb();
        void    simpleBlobDetector();

    private:

        void    initImageTypes();

        int     getChannelsFromType(int type);

        void    enableAllImageTypes();
        void    enableImageTypeFromDepth(std::vector<int> depthList);
        void    enableImageTypeFromChannels(int channels);
        void    enableImageTypeFromType(std::vector<int> types);

        CMat    convertImage(const CMat& src, int newType);

        void    addGraphicsRect(std::shared_ptr<CGraphicsProcessInput>& graphicsInputPtr);

        CMat    loadSampleImage();

        void    runNoInput(const std::string algoName, const ProtocolTaskParamPtr &paramPtr);
        void    runImage(const std::string algoName, const ProtocolTaskParamPtr &paramPtr, int inputIndex, int graphicsInputIndex=-1);
        void    runVideo(const std::string algoName, const ProtocolTaskParamPtr &paramPtr, int inputIndex, int graphicsInputIndex=-1);
        void    runTypeImageTest(const std::string algoName, const ProtocolTaskParamPtr &paramPtr, int inputIndex);
        void    runStdTest1(const std::string algoName, const ProtocolTaskParamPtr &paramPtr, int inputIndex, int graphicsInputIndex);
        void    runOpenCL(const std::string algoName, const ProtocolTaskParamPtr &paramPtr, int inputIndex);

    private:

        CMat                                m_image;
        QMap<int,bool>                      m_imgTypes;
        const size_t                        m_maxFrames = 50;
        std::shared_ptr<CVideoProcessIO>    m_videoInputPtr;
        CProcessRegistration                m_processRegister;
};

#endif // COCVPROCESSTESTS_H
