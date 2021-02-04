#define NO_IMPORT_ARRAY
#include "CDataStructureTests.h"
#include <QTest>
#include "UnitTestUtils.hpp"
#include "Data/CndArray.hpp"
#include "Data/CDataset.hpp"
#include "Data/CDataConversion.h"
#include "CDataIO.hpp"
#include "CImageDataManager.h"

CDataStructureTests::CDataStructureTests(QObject *parent) : QObject(parent)
{
}

std::string CDataStructureTests::sampleImgPath()
{
    std::string imagePath = UnitTest::getDataPath() + "/Images/Lena.png";
    return imagePath;
}

Dimensions CDataStructureTests::dimensions(const SubsetBounds& bounds)
{
    Dimensions dims;
    for(size_t i=0; i<bounds.size(); ++i)
        dims.push_back(std::make_pair(bounds[i].first, bounds[i].second.second - bounds[i].second.first + 1));

    return dims;
}

void CDataStructureTests::ndArrayConstructors()
{
    const size_t nbModality = 4;
    const size_t nbZ = 100;

    CndArray<float> emptyArray;
    QVERIFY(emptyArray.size() == 0);

    Dimensions dims = {{DataDimension::MODALITY, nbModality}, {DataDimension::VOLUME, 1}, {DataDimension::IMAGE, nbZ}};
    CndArray<float> array1(dims);
    QVERIFY(array1.size() == nbModality*nbZ);
    QVERIFY(array1.dimensionsCount() == 3);
    QVERIFY(array1.size(DataDimension::MODALITY) == nbModality);
    QVERIFY(array1.size(DataDimension::IMAGE) == nbZ);

    CndArray<float> array2(array1);
    QVERIFY(array2.size() == nbModality*nbZ);
    QVERIFY(array2.dimensionsCount() == 3);
    QVERIFY(array2.size(DataDimension::MODALITY) == nbModality);
    QVERIFY(array2.size(DataDimension::IMAGE) == nbZ);

    CndArray<float> array3(std::move(array1));
    QVERIFY(array3.size() == nbModality*nbZ);
    QVERIFY(array3.dimensionsCount() == 3);
    QVERIFY(array3.size(DataDimension::MODALITY) == nbModality);
    QVERIFY(array3.size(DataDimension::IMAGE) == nbZ);
}

void CDataStructureTests::ndArrayAccessors()
{
    const size_t nbModality = 4;
    const size_t nbImage = 100;
    Dimensions dims = {{DataDimension::MODALITY, nbModality}, {DataDimension::VOLUME, 1}, {DataDimension::IMAGE, nbImage}};
    CndArray<std::string> array(dims);

    for(size_t i=0; i<nbModality; ++i)
    {
        for(size_t j=0; j<nbImage; ++j)
        {
            std::string fileName = "image_" + std::to_string(i) + "_" + std::to_string(j);
            array[i*nbImage + j] = fileName;
        }
    }

    //Check valid index
    QVERIFY(array[0] == array.at(0));

    DimensionIndices indices(dims.size());
    indices[0] = {DataDimension::MODALITY, 0};
    indices[1] = {DataDimension::VOLUME, 0};
    indices[2] = {DataDimension::IMAGE, 0};
    QVERIFY(array[0] == array.at(indices));
    std::cout << array.at(indices) << std::endl;

    indices[0] = {DataDimension::MODALITY, 2};
    indices[1] = {DataDimension::VOLUME, 0};
    indices[2] = {DataDimension::IMAGE, 39};
    QVERIFY(array[2*nbImage+39] == array.at(indices));
    std::cout << array.at(indices) << std::endl;

    //Check invalid index
    QVERIFY_EXCEPTION_THROWN(array[SIZE_MAX], CException);

    indices[0] = {DataDimension::MODALITY, 0};
    indices[1] = {DataDimension::VOLUME, 0};
    indices[2] = {DataDimension::IMAGE, 11000};
    QVERIFY_EXCEPTION_THROWN(array.at(indices), CException);
}

void CDataStructureTests::ndArraySubArrayCreation()
{
    CndArray<std::string> array, subArray;
    const size_t nbModality = 4;
    const size_t nbVolume = 1;
    const size_t nbZ = 20;
    Dimensions dims = {{DataDimension::MODALITY, nbModality}, {DataDimension::VOLUME, nbVolume}, {DataDimension::IMAGE, nbZ}};
    array.setDimensions(dims);

    for(size_t i=0; i<nbModality; ++i)
    {
        for(size_t j=0; j<nbZ; ++j)
        {
            std::string fileName = "image_" + std::to_string(i) + "_" + std::to_string(j);
            array[i*nbZ + j] = fileName;
        }
    }

    std::cout << "----- Original set ----" << std::endl;
    for(size_t i=0; i<array.size(); ++i)
        std::cout << array[i] << std::endl;

    SubsetBounds subset = {{DataDimension::MODALITY, {0, 2}},
                           {DataDimension::VOLUME, {0, 0}},
                           {DataDimension::IMAGE, {2, 11}}};

    QBENCHMARK
    {
        subArray = array.subArray(subset);
    }
    QVERIFY(subArray.size() == 30);
    QVERIFY(subArray.dimensionsCount() == 3);
    QVERIFY(subArray.size(DataDimension::MODALITY) == 3);
    QVERIFY(subArray.size(DataDimension::VOLUME) == 1);
    QVERIFY(subArray.size(DataDimension::IMAGE) == 10);

    std::cout << "----- Subset ----" << std::endl;
    for(size_t i=0; i<subArray.size(); ++i)
        std::cout << subArray[i] << std::endl;
}

void CDataStructureTests::ndArrayUpDimension()
{
    const size_t nbT = 3;
    const size_t nbPos = 5;
    const size_t nbVolume = 1;
    const size_t nbZ = 10;
    const size_t nbUp = 2;

    Dimensions dims = {{DataDimension::TIME, nbT},
                       {DataDimension::POSITION, nbPos},
                       {DataDimension::VOLUME, nbVolume},
                       {DataDimension::IMAGE, nbZ}};

    CndArray<int> array, arrayInc;

    array.setDimensions(dims);
    for(size_t i=0; i<nbT*nbPos*nbVolume*nbZ; ++i)
        array[i] = i;

    arrayInc = array;
    arrayInc.upDimension(DataDimension::TIME, nbUp);
    QVERIFY(arrayInc.size(DataDimension::TIME) == nbT + nbUp);
    QVERIFY(arrayInc.size() == (nbT+nbUp)*nbPos*nbVolume*nbZ);

    arrayInc = array;
    arrayInc.upDimension(DataDimension::POSITION, nbUp);
    QVERIFY(arrayInc.size(DataDimension::POSITION) == nbPos + nbUp);
    QVERIFY(arrayInc.size() == nbT*(nbPos+nbUp)*nbVolume*nbZ);

    arrayInc = array;
    arrayInc.upDimension(DataDimension::VOLUME, nbUp);
    QVERIFY(arrayInc.size(DataDimension::VOLUME) == nbVolume + nbUp);
    QVERIFY(arrayInc.size() == nbT*nbPos*(nbVolume+nbUp)*nbZ);

    arrayInc = array;
    arrayInc.upDimension(DataDimension::IMAGE, nbUp);
    QVERIFY(arrayInc.size(DataDimension::IMAGE) == nbZ + nbUp);
    QVERIFY(arrayInc.size() == nbT*nbPos*nbVolume*(nbZ+nbUp));

    arrayInc = array;
    arrayInc.upDimension(DataDimension::MODALITY, nbUp);
    QVERIFY(arrayInc.size(DataDimension::MODALITY) == nbUp);
    QVERIFY(arrayInc.size() == nbT*nbPos*nbVolume*nbZ*nbUp);
}

void CDataStructureTests::ndArrayDownDimension()
{
    const size_t nbT = 3;
    const size_t nbPos = 5;
    const size_t nbVolume = 1;
    const size_t nbZ = 10;
    const size_t nbDown = 2;

    Dimensions dims = {{DataDimension::TIME, nbT},
                       {DataDimension::POSITION, nbPos},
                       {DataDimension::VOLUME, nbVolume},
                       {DataDimension::IMAGE, nbZ}};

    CndArray<int> array, arrayDec;

    array.setDimensions(dims);
    for(size_t i=0; i<nbT*nbPos*nbVolume*nbZ; ++i)
        array[i] = i;

    arrayDec = array;
    arrayDec.downDimension(DataDimension::TIME, nbDown);
    QVERIFY(arrayDec.size(DataDimension::TIME) == nbT - nbDown);
    QVERIFY(arrayDec.size() == (nbT-nbDown)*nbPos*nbVolume*nbZ);

    arrayDec = array;
    arrayDec.downDimension(DataDimension::POSITION, nbDown);
    QVERIFY(arrayDec.size(DataDimension::POSITION) == nbPos - nbDown);
    QVERIFY(arrayDec.size() == nbT*(nbPos-nbDown)*nbVolume*nbZ);

    arrayDec = array;
    arrayDec.downDimension(DataDimension::VOLUME, nbDown);
    QVERIFY(arrayDec.size(DataDimension::VOLUME) == nbVolume);
    QVERIFY(arrayDec.size() == nbT*nbPos*nbVolume*nbZ);

    arrayDec = array;
    arrayDec.downDimension(DataDimension::IMAGE, nbDown);
    QVERIFY(arrayDec.size(DataDimension::IMAGE) == nbZ - nbDown);
    QVERIFY(arrayDec.size() == nbT*nbPos*nbVolume*(nbZ-nbDown));

    arrayDec = array;
    arrayDec.downDimension(DataDimension::MODALITY, nbDown);
    QVERIFY(arrayDec.size(DataDimension::MODALITY) == 0);
    QVERIFY(arrayDec.size() == nbT*nbPos*nbVolume*nbZ);
}

void CDataStructureTests::ndArrayInsertDimension()
{
    const size_t nbRows = 3;
    const size_t nbCols = 5;
    Dimensions dims = {{DataDimension::Y, nbRows},
                       {DataDimension::X, nbCols}};

    CndArray<int> array(dims);
    for(size_t i=0; i<nbRows*nbCols; ++i)
        array[i] = i;

    array.insertDimension(DataDimension::Y, 1);
    QVERIFY(array.size(DataDimension::Y) == nbRows + 1);

    array.insertDimension(DataDimension::X, 3);
    QVERIFY(array.size(DataDimension::X) == nbCols + 1);
}

void CDataStructureTests::datasetConstructors()
{
    //Main constructor
    CDataset<CMat> dataset(IODataType::IMAGE);
    QVERIFY(dataset.getType() == IODataType::IMAGE);

    const size_t nbImage = 20;
    Dimensions dims = {{DataDimension::IMAGE, nbImage}};
    dataset.setDimensions(dims);

    for(size_t i=0; i<nbImage; ++i)
    {
        std::string fileName = "image_" + std::to_string(i);
        dataset.appendFile(fileName);
    }
    QVERIFY(dataset.size() == nbImage);

    //Copy constructor
    CDataset<CMat> dataset2(dataset);
    QVERIFY(dataset2.getType() == dataset.getType());
    QVERIFY(dataset2.getInfo() == dataset.getInfo());
    QVERIFY(dataset2.size() == dataset.size());

    //Assignement
    auto dataset3 = dataset;
    QVERIFY(dataset3.getType() == dataset.getType());
    QVERIFY(dataset3.getInfo() == dataset.getInfo());
    QVERIFY(dataset3.size() == dataset.size());
}

void CDataStructureTests::datasetAccessors()
{
    CDataset<CMat> dataset(IODataType::IMAGE);
    const size_t nbImage = 20;
    Dimensions dims = {{DataDimension::IMAGE, nbImage}};
    dataset.setDimensions(dims);

    //Invalid tests
    QVERIFY_EXCEPTION_THROWN(dataset.dataAt(0), CException);

    DimensionIndices indices = {{DataDimension::IMAGE, 50}};
    QVERIFY_EXCEPTION_THROWN(dataset.dataAt(indices), CException);
}

void CDataStructureTests::datasetIndexConversion()
{
    const size_t nbM = 5;
    const size_t nbT = 10;
    const size_t nbVolume = 1;
    const size_t nbZ = 20;

    //Fill dataset
    CDataset<std::string> dataset(IODataType::IMAGE);
    Dimensions dims = {{DataDimension::MODALITY, nbM},
                       {DataDimension::TIME, nbT},
                       {DataDimension::VOLUME, nbVolume},
                       {DataDimension::IMAGE, nbZ}};

    dataset.setCurrentIndex(0);
    dataset.setDimensions(dims);

    for(size_t i=0; i<nbM; ++i)
    {
        for(size_t j=0; j<nbT; ++j)
        {
            for(size_t k=0; k<nbVolume; ++k)
            {
                for(size_t l=0; l<nbZ; ++l)
                {
                    std::string fileName = "image_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k) + "_" + std::to_string(l);
                    dataset.appendFile(fileName);
                }
            }
        }
    }

    //Fill subset
    SubsetBounds bounds = {{DataDimension::MODALITY, {0, 2}},
                           {DataDimension::TIME, {1, 5}},
                           {DataDimension::VOLUME, {0, 0}},
                           {DataDimension::IMAGE, {5, 14}}};

    dataset.subset().setBounds(bounds);
    dataset.subset().setDimensions(dimensions(bounds));

    for(size_t i=bounds[0].second.first; i<=bounds[0].second.second; ++i)
    {
        for(size_t j=bounds[1].second.first; j<=bounds[1].second.second; ++j)
        {
            for(size_t k=bounds[2].second.first; k<=bounds[2].second.second; ++k)
            {
                for(size_t l=bounds[3].second.first; l<=bounds[3].second.second; ++l)
                {
                    std::string fileName = "image_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k) + "_" + std::to_string(l);
                    dataset.subset().addData(fileName);
                }
            }
        }
    }

    //Check subset to dataset conversion
    std::cout << "---------- Subset to Dataset conversion ----------" << std::endl;
    for(size_t i=0; i<dataset.subset().size(); ++i)
    {
        size_t index = dataset.subsetToDatasetIndex(i);
        if(index == SIZE_MAX)
            QFAIL("Bad index conversion");
        else
        {
            std::cout << "Dataset = " << dataset[index]->getFileName() << " -- Subset == " << dataset.subset()[i] << std::endl;
            QVERIFY(dataset[index]->getFileName() == dataset.subset()[i]);
        }
    }

    //Check dataset to subset conversion
    std::cout << "---------- Dataset to Subset conversion ----------" << std::endl;
    for(size_t i=0; i<dataset.size(); ++i)
    {
        size_t index = dataset.datasetToSubsetIndex(i);
        if(index == SIZE_MAX)
            std::cout << "Index out of bounds: " << std::to_string(i) << std::endl;
        else
        {
            std::cout << "Dataset = " << dataset[i]->getFileName() << " -- Subset == " << dataset.subset()[index] << std::endl;
            QVERIFY(dataset[i]->getFileName() == dataset.subset()[index]);
        }
    }
}

void CDataStructureTests::dataConversion()
{
    int width=512, height=512;
    CDataConversion conversion;
    std::vector<int> cvTypes = {    CV_8UC1, CV_8UC3, CV_8UC4,
                                    CV_16SC1, CV_16SC3, CV_16SC4,
                                    CV_16UC1, CV_16UC3, CV_16UC4,
                                    CV_32SC1, CV_32SC3, CV_32SC4,
                                    CV_32FC1, CV_32FC3, CV_32FC4,
                                    CV_64FC1, CV_64FC3, CV_64FC4
                                };

    std::vector<int> channelCounts = {  1, 3, 4,
                                        1, 3, 4,
                                        1, 3, 4,
                                        1, 3, 4,
                                        1, 3, 4,
                                        1, 3, 4
                                     };

    for(size_t i=0; i<cvTypes.size(); ++i)
    {
        bool bGray = (  cvTypes[i] == CV_8UC1 || cvTypes[i] == CV_16SC1 || cvTypes[i] == CV_16UC1 ||
                        cvTypes[i] == CV_32SC1 || cvTypes[i] == CV_32FC1 || cvTypes[i] == CV_64FC1);
        bool bAlpha = ( cvTypes[i] == CV_8UC4 || cvTypes[i] == CV_16SC4 || cvTypes[i] == CV_16UC4 ||
                        cvTypes[i] == CV_32SC4 || cvTypes[i] == CV_32FC4 || cvTypes[i] == CV_64FC4);

        cv::Scalar pxColor;
        if(bGray)
            pxColor = cv::Scalar(0);
        else if(bAlpha)
            pxColor = cv::Scalar(255, 0, 0, 255);
        else
            pxColor = cv::Scalar(255, 0, 0, 0);

        CMat mat(width, height, cvTypes[i], pxColor);
        auto qimg = conversion.CMatToQImage(mat, false);

        QVERIFY(qimg.isNull() == false);
        QVERIFY(qimg.width() == width);
        QVERIFY(qimg.height() == height);
        QVERIFY(qimg.isGrayscale() == bGray);
        QVERIFY(qimg.hasAlphaChannel() == bAlpha);
        QVERIFY(qimg.depth() == channelCounts[i]*8);
    }
}

void CDataStructureTests::loadSingleImage()
{
    std::string imagePath = sampleImgPath();
    IODataType IODataType = _CDataIO::dataType(imagePath);

    if(IODataType == IODataType::IMAGE)
    {
        CDataset<CMat> dataset(IODataType);

        QBENCHMARK
        {
            Dimensions dims = {{DataDimension::IMAGE, 1}};
            dataset.clear();
            dataset.setDimensions(dims);
            dataset.appendFile(imagePath);

            CImageDataManager dataMgr;
            dataMgr.loadData(dataset);
            QVERIFY2(dataset.subset().size() == 1, "Invalid number of images");
        }

        CMat validationImg = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
        if(validationImg.channels() > 1)
            cv::cvtColor(validationImg, validationImg, cv::COLOR_BGR2RGB);

        CMat testImg = dataset.dataAt(0);
        QVERIFY2(testImg.empty() == false, "Data structure error: image is empty");

#ifndef Q_OS_WIN
        bool bEqual = std::equal(testImg.begin<uchar>(), testImg.end<uchar>(), validationImg.begin<uchar>(), validationImg.end<uchar>());
        QVERIFY2(bEqual == true, "DataStructure error: mismatch with validation image");
#endif
    }
}

void CDataStructureTests::loadMultiImages()
{
    std::string imagePath = sampleImgPath();
    IODataType IODataType = _CDataIO::dataType(imagePath);

    if(IODataType == IODataType::IMAGE)
    {
        QBENCHMARK
        {
            const size_t nbImages = 10;
            CDataset<CMat> dataset(IODataType);
            Dimensions dims = {{DataDimension::IMAGE, nbImages}};
            dataset.setCurrentIndex(0);
            dataset.setDimensions(dims);

            for(size_t i=0; i<nbImages; ++i)
                dataset.appendFile(imagePath);

            CImageDataManager dataMgr;
            dataMgr.loadData(dataset);
            QVERIFY2(dataset.subset().size() == nbImages, "Invalid number of images");
        }
    }
}

void CDataStructureTests::loadZStackImages()
{
    std::string imagePath = sampleImgPath();
    IODataType IODataType = _CDataIO::dataType(imagePath);

    if(IODataType == IODataType::IMAGE)
    {
        QBENCHMARK
        {
            const size_t nbImages = 10;
            CDataset<CMat> dataset(IODataType);
            Dimensions dims = {{DataDimension::VOLUME, 1}, {DataDimension::IMAGE, nbImages}};
            dataset.setCurrentIndex(0);
            dataset.setDimensions(dims);

            for(size_t i=0; i<nbImages; ++i)
                dataset.appendFile(imagePath);

            CImageDataManager dataMgr;
            dataMgr.loadData(dataset);
            QVERIFY2(dataset.subset().size() == 1, "Invalid number of images");
        }
    }
}

void CDataStructureTests::loadNdImages()
{
    std::string imagePath = sampleImgPath();
    IODataType IODataType = _CDataIO::dataType(imagePath);

    if(IODataType == IODataType::IMAGE)
    {
        QBENCHMARK
        {
            const size_t nbM = 5;
            const size_t nbT = 10;
            const size_t nbVolume = 1;
            const size_t nbZ = 20;

            CDataset<CMat> dataset(IODataType);
            Dimensions dims = {{DataDimension::MODALITY, nbM},
                               {DataDimension::TIME, nbT},
                               {DataDimension::VOLUME, nbVolume},
                               {DataDimension::IMAGE, nbZ}};

            dataset.setCurrentIndex(0);
            dataset.setDimensions(dims);

            for(size_t i=0; i<nbM*nbT*nbVolume*nbZ; ++i)
                dataset.appendFile(imagePath);

            CImageDataManager dataMgr;
            dataMgr.loadData(dataset);
            QVERIFY2(dataset.subset().size() == nbM*nbT*nbVolume, "Invalid number of images");
        }
    }
}

QTEST_GUILESS_MAIN(CDataStructureTests)
