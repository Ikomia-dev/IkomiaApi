#include "CDataInfoTests.h"
#include <QTest>
#include "Data/CDataArrayInfo.h"

CDataInfoTests::CDataInfoTests(QObject* parent) : QObject(parent)
{
}

void CDataInfoTests::arrayTest()
{
    std::vector<int> sizes = {32, 64};

    CDataArrayInfo info;
    auto strInfo = info.getStringList();
    QVERIFY(!strInfo.empty());

    //2D array
    try
    {
        CMat array2d(sizes[0], sizes[1], CV_8UC1, cv::Scalar(0));
        info.updateArray(array2d);
        strInfo = info.getStringList();
        QVERIFY(!strInfo.empty());
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    //3D array
    try
    {
        sizes.push_back(128);
        CMat array3d(sizes, CV_16UC1, cv::Scalar(1));
        info.updateArray(array3d);
        strInfo = info.getStringList();
        QVERIFY(!strInfo.empty());
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    //4D array
    try
    {
        sizes.push_back(256);
        CMat array4d(sizes, CV_32FC1, cv::Scalar(1.0));
        info.updateArray(array4d);
        strInfo = info.getStringList();
        QVERIFY(!strInfo.empty());
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    try
    {
        CMat array4d(sizes, CV_32FC1, cv::Scalar(1.0));
        CDataArrayInfo info4d(array4d);
        info4d.updateArray(array4d);
        strInfo = info4d.getStringList();
        QVERIFY(!strInfo.empty());
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    try
    {
        CMat array4d(sizes, CV_64FC1, cv::Scalar(1.0));
        CDataArrayInfo info4d(array4d);
        info4d.updateArray(array4d);
        strInfo = info4d.getStringList();
        QVERIFY(!strInfo.empty());
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }

    //Multi-channel array
    try
    {
        CMat array4d(sizes, CV_64FC3, cv::Scalar(1.0));
        CDataArrayInfo info4d(array4d);
        info4d.updateArray(array4d);
        strInfo = info4d.getStringList();
        QVERIFY(!strInfo.empty());
    }
    catch(std::exception& e)
    {
        QFAIL(e.what());
    }
}

QTEST_GUILESS_MAIN(CDataInfoTests)
