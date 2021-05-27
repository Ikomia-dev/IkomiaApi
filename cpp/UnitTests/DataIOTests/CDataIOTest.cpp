#include "CDataIOTest.h"
#include <QTest>
#include <opencv2/opencv.hpp>
#include <CDataIO.hpp>
#include "DataIOTools.hpp"
#include "Data/CDataImageInfo.h"
#include "Data/CDataVideoInfo.h"
#include "UnitTestUtils.hpp"

CDataIOTest::CDataIOTest(QObject *parent) : QObject(parent)
{
}

void CDataIOTest::readOpencvImage()
{
    try
    {
        // Test wrong file path
        CDataIO<CDataImageIO, CMat> imageIO_1("");
        QVERIFY_EXCEPTION_THROWN(imageIO_1.read(), std::exception);

        //Test valid image reading
        CMat img;
        std::string imagePath = UnitTest::getDataPath() + "/Images/Lena.png";
        CDataIO<CDataImageIO, CMat> imageIO_2(imagePath);

        QBENCHMARK
        {
            img = imageIO_2.read();
            QVERIFY(img.data != nullptr);
        }

        //Dimensions checking
        Dimensions dims = imageIO_2.dimensions();
        QVERIFY(dims.size() > 0);

        //Check data info
        auto dataInfoPtr = imageIO_2.dataInfo();
        QVERIFY(dataInfoPtr != nullptr);
        QVERIFY(dataInfoPtr->getElementSize() == 1);
        QVERIFY(dataInfoPtr->getFileName() == imagePath);
        QVERIFY(dataInfoPtr->getType() == IODataType::IMAGE);

        auto imgDataInfoPtr = std::dynamic_pointer_cast<CDataImageInfo>(dataInfoPtr);
        QVERIFY(imgDataInfoPtr != nullptr);
        QVERIFY(imgDataInfoPtr->m_nbBit == 8);
        QVERIFY(imgDataInfoPtr->m_bitMin == 0);
        QVERIFY(imgDataInfoPtr->m_bitMax == 7);
        QVERIFY(imgDataInfoPtr->m_minValue == 0);
        QVERIFY(imgDataInfoPtr->m_maxValue == 255);
        QVERIFY(imgDataInfoPtr->m_nbBand == 3);
        QVERIFY(imgDataInfoPtr->m_width == 512);
        QVERIFY(imgDataInfoPtr->m_height == 512);
        QVERIFY(imgDataInfoPtr->m_cvType == CV_8UC3);

        //Check image content
#ifndef Q_OS_WIN
        CMat validationImg = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
        if(img.channels() > 1)
            cv::cvtColor(img, img, cv::COLOR_BGR2RGB);

        bool bEqual = false;

        //CMat imgDiff;
        //cv::absdiff(img, validationImg, imgDiff);
        //auto numDiff = cv::countNonZero(imgDiff);
        //bEqual = numDiff == 0;

        bEqual = std::equal(img.begin<uchar>(), img.end<uchar>(), validationImg.begin<uchar>(), validationImg.end<uchar>());
        QVERIFY2(bEqual == true, "DataIO error: mismatch with validation image");
#endif
    }
    catch(CException& e)
    {
        QFAIL(e.what());
    }
}

void CDataIOTest::readVideoFrame()
{
    try
    {
        // Test wrong file path
        CDataIO<CDataImageIO, CMat> videoIO_1("");
        QVERIFY_EXCEPTION_THROWN(videoIO_1.read(), std::exception);

        //Test valid video reading
        std::string videoPath = UnitTest::getDataPath() + "/Videos/video.avi";
        CDataIO<CDataVideoIO, CMat> videoIO_2(videoPath);

        //Dimensions checking
        Dimensions dims = videoIO_2.dimensions();
        QVERIFY(dims.size() > 0);

        QBENCHMARK
        {
            CMat img = videoIO_2.read();
            QVERIFY(img.data != nullptr);
        }

        //Check data info
        auto dataInfoPtr = videoIO_2.dataInfo();
        QVERIFY(dataInfoPtr != nullptr);
        auto videoDataInfoPtr = std::dynamic_pointer_cast<CDataVideoInfo>(dataInfoPtr);
        QVERIFY(videoDataInfoPtr->getElementSize() == 1);
        QVERIFY(videoDataInfoPtr->getFileName() == videoPath);
        QVERIFY(videoDataInfoPtr->getType() == IODataType::VIDEO);
        QVERIFY(videoDataInfoPtr->m_nbBit == 8);
        QVERIFY(videoDataInfoPtr->m_bitMin == 0);
        QVERIFY(videoDataInfoPtr->m_bitMax == 7);
        QVERIFY(videoDataInfoPtr->m_minValue == 0);
        QVERIFY(videoDataInfoPtr->m_maxValue == 255);
        QVERIFY(videoDataInfoPtr->m_nbBand == 3);
        QVERIFY(videoDataInfoPtr->m_width == 320);
        QVERIFY(videoDataInfoPtr->m_height == 240);
        QVERIFY(videoDataInfoPtr->m_cvType == CV_8UC3);
        QVERIFY(videoDataInfoPtr->m_fps == 60);
        QVERIFY(videoDataInfoPtr->m_frameCount == 1700);
    }
    catch(CException& e)
    {
        QFAIL(e.what());
    }
}

void CDataIOTest::playVideo()
{
    try
    {
        // Threaded video reading
        std::string videoPath = UnitTest::getDataPath() + "/Videos/video.avi";
        CDataIO<CDataVideoIO, CMat> videoIO(videoPath);

        // Play video
        auto videoDataInfoPtr = std::dynamic_pointer_cast<CDataVideoInfo>(videoIO.dataInfo());
        QVERIFY(videoDataInfoPtr != nullptr);

        // framecount - 1  because the frame count is not well estimated for our test video
        for(int i=0; i<videoDataInfoPtr->m_frameCount-1; ++i)
        {
            CMat frame = videoIO.readLive();
            QVERIFY(frame.data != nullptr);
        }

        videoIO.stopRead();
    }
    catch(CException& e)
    {
        QFAIL(e.what());
    }
}

void CDataIOTest::saveOpencvImage()
{
    std::string srcImagePath = UnitTest::getDataPath() + "/Images/Lena.png";
    CDataIO<CDataImageIO, CMat> imageReader(srcImagePath);
    CMat img = imageReader.read();
    QVERIFY(img.data != nullptr);

    std::string pathPattern = UnitTest::getDataPath() + "/Images/tmp";
    auto extensions = CDataImageIO::getOpencvExtensions();

    for(auto it=extensions.begin(); it!=extensions.end(); ++it)
    {
        std::string path = pathPattern + *it;
        try
        {
            CDataIO<CDataImageIO, CMat> imageWriter(path);
            imageWriter.write(img);

            if(boost::filesystem::exists(path))
            {
                boost::filesystem::path boostPath(path);
                boost::filesystem::remove(boostPath);
            }
            else
            {
                std::string err = "Image not saved: " + path;
                QWARN(err.c_str());
            }
        }
        catch(std::exception& e)
        {
            std::string err = "Error on image: " + path + " - " + e.what();
            QWARN(err.c_str());
        }
    }
}

void CDataIOTest::writeVideo()
{
    std::string srcImagePath = UnitTest::getDataPath() + "/Images/Lena.png";
    CDataIO<CDataImageIO, CMat> imageIO(srcImagePath);
    CMat frame = imageIO.read();
    QVERIFY(frame.data != nullptr);

    // Write AVI
    std::string videoPath = UnitTest::getDataPath() + "/Videos/tmp.avi";
    CDataIO<CDataVideoIO, CMat> videoIO_1(videoPath);

    const int frameCount = 500;
    for(int i=0; i<frameCount; ++i)
        videoIO_1.write(frame);

    videoIO_1.waitWriteFinished();
    QVERIFY(boost::filesystem::exists(videoPath));
    boost::filesystem::path boostPath1(videoPath);
    boost::filesystem::remove(boostPath1);

    // Write MPG4
    videoPath = UnitTest::getDataPath() + "/Videos/tmp.mp4";
    CDataIO<CDataVideoIO, CMat> videoIO_2(videoPath);

    for(int i=0; i<frameCount; ++i)
        videoIO_2.write(frame);

    videoIO_2.waitWriteFinished();
    QVERIFY(boost::filesystem::exists(videoPath));
    boost::filesystem::path boostPath2(videoPath);
    boost::filesystem::remove(boostPath2);
}

void CDataIOTest::testTools()
{
    auto path = UnitTest::getDataPath() + "/Images/Lena.png";
    QVERIFY(boost::filesystem::exists(path) == true);
    path = UnitTest::getDataPath() + "/Videos/video.avi";
    QVERIFY(boost::filesystem::exists(path) == true);
}

QTEST_GUILESS_MAIN(CDataIOTest)
