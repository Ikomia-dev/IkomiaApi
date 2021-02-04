#include "CDicomImageIO.h"
#include <vtkAutoInit.h>
#include <vtkStringArray.h>
#include <vtkDICOMReader.h>
#include <vtkDICOMDictionary.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include "DataIOTools.hpp"
#include "Data/CDataDicomInfo.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

CDicomImageIO::CDicomImageIO(const std::string& path) : CVirtualImageIO(path)
{
    std::string folder;
    if(boost::filesystem::is_regular_file(path))
    {
        boost::filesystem::path boostPath(path);
        folder = boostPath.parent_path().string();
        m_currentFile = path;
    }
    else
    {
        folder = path;
        m_currentFile.clear();
        fillFilesList(folder);
        sortFiles();
    }    
}

CDicomImageIO::~CDicomImageIO()
{
}

VectorString CDicomImageIO::fileNames(const SubsetBounds &bounds)
{
    if(!isValidSubsetBounds(bounds))
        return VectorString();
    else
    {
        //Get first serie index for given study
        int firstSerie = m_pDicomSorter->GetFirstSeriesForStudy(static_cast<int>(Utils::Data::getDimensionBounds(bounds, DataDimension::STUDY).first));
        //Get all file names for given serie
        vtkStringArray* vtkFiles = m_pDicomSorter->GetFileNamesForSeries(firstSerie + static_cast<int>(Utils::Data::getDimensionBounds(bounds, DataDimension::SERIE).first));

        VectorString files;
        for(int i=0; i<vtkFiles->GetNumberOfValues(); ++i)
            files.push_back(vtkFiles->GetValue(i));

        return files;
    }
}

Dimensions CDicomImageIO::dimensions()
{
    return dimensions(SubsetBounds());
}

Dimensions CDicomImageIO::dimensions(const SubsetBounds &bounds)
{
    assert(m_pDicomSorter != nullptr);

    if(m_pDicomSorter->GetInputFileNames()->GetNumberOfValues() == 0)
        return Dimensions();
    else
    {
        if(m_currentFile.empty() == false)
        {
            Dimensions dims = {{DataDimension::IMAGE, 1}};
            return dims;
        }
        else
        {
            Dimensions dims = {{DataDimension::STUDY, m_pDicomSorter->GetNumberOfStudies()},
                               {DataDimension::SERIE, m_pDicomSorter->GetNumberOfSeries()},
                               {DataDimension::VOLUME, 1}};

            if(isValidSubsetBounds(bounds) == false)
            {
                //Get first serie index for the first study
                int firstSerie = m_pDicomSorter->GetFirstSeriesForStudy(0);
                //Get all file names for the first serie
                vtkStringArray* files = m_pDicomSorter->GetFileNamesForSeries(firstSerie);
                //Set the number of images
                dims.push_back(std::make_pair(DataDimension::IMAGE, files->GetNumberOfValues()));
            }
            else
            {
                //Get first serie index for given study
                int firstSerie = m_pDicomSorter->GetFirstSeriesForStudy(static_cast<int>(Utils::Data::getDimensionBounds(bounds, DataDimension::STUDY).first));
                //Get all file names for given serie
                vtkStringArray* files = m_pDicomSorter->GetFileNamesForSeries(firstSerie + static_cast<int>(Utils::Data::getDimensionBounds(bounds, DataDimension::SERIE).first));
                //Set the number of images
                dims.push_back(std::make_pair(DataDimension::IMAGE, files->GetNumberOfValues()));
            }
            return dims;
        }
    }
}

CDataInfoPtr CDicomImageIO::dataInfo()
{
    return dataInfo(SubsetBounds());
}

CDataInfoPtr CDicomImageIO::dataInfo(const SubsetBounds &bounds)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto pVtkImgData = vtkImgData(bounds);
    if(pVtkImgData == nullptr)
        return CDataInfoPtr();

    auto pInfo = std::make_shared<CDataDicomInfo>();
    assert(pInfo != nullptr);

    if(m_pDicomSorter)
    {
        //Study
        pInfo->m_nbStudy = m_pDicomSorter->GetNumberOfStudies();
        //Serie
        pInfo->m_nbSerie = seriesCount(bounds);
    }

    //Filename
    pInfo->setFileName(m_fileName);
    //Width - Height
    int* pDims = pVtkImgData->GetDimensions();
    assert(pDims != nullptr);
    pInfo->m_width = pDims[0];
    pInfo->m_height = pDims[1];
    //Nb band
    pInfo->m_nbBand = pVtkImgData->GetNumberOfScalarComponents();
    //Nb bit
    pInfo->m_nbBit = pVtkImgData->GetScalarSize() * 8;
    //cvType
    pInfo->m_cvType = cvType(pVtkImgData);

    auto pVtkMetadata = vtkMetadata(bounds);
    if(pVtkMetadata == nullptr)
        return pInfo;

    // Get and transfer all DICOM metadata
    for(vtkDICOMDataElementIterator it = pVtkMetadata->Begin(); it != pVtkMetadata->End(); ++it)
    {
        vtkDICOMTag tag = it->GetTag();
        vtkDICOMDictEntry entry;

        entry = vtkDICOMDictionary::FindDictEntry(tag);
        if(it->IsPerInstance())
        {
            int nb = it->GetNumberOfInstances();
            for(int i = 0; i < nb; ++i)
                pInfo->metadata().insert(make_pair(entry.GetName(), it->GetValue().AsString()));
        }
        else
            pInfo->metadata().insert(make_pair(entry.GetName(), it->GetValue().AsString()));
    }
    return pInfo;
}

CMat CDicomImageIO::read()
{
    return read(SubsetBounds());
}

CMat CDicomImageIO::read(const SubsetBounds &bounds)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto pVtkImageData = vtkImgData(bounds);
    if(pVtkImageData == nullptr)
        return CMat();

    auto pBuffer = pVtkImageData->GetScalarPointer();
    if(pBuffer == nullptr)
        return CMat();

    int type = cvType(pVtkImageData);
    int* dims = pVtkImageData->GetDimensions();

    vtkIdType dx, dy, dz;
    pVtkImageData->GetIncrements(dx, dy, dz);
    CMat localImg = CMat(dims[1], dims[0], type, pBuffer, dy * pVtkImageData->GetScalarSize());
    CMat img = localImg.clone();
    cv::flip(img, img, 0);
    return img;
}

bool CDicomImageIO::isValidSubsetBounds(const SubsetBounds &bounds)
{
    Bounds invalidBounds = {-1, -1};
    return (Utils::Data::getDimensionBounds(bounds, DataDimension::STUDY) != invalidBounds &&
            Utils::Data::getDimensionBounds(bounds, DataDimension::SERIE) != invalidBounds &&
            Utils::Data::getDimensionBounds(bounds, DataDimension::IMAGE) != invalidBounds);
}

void CDicomImageIO::fillFilesList(const std::string &folder)
{
    if(!boost::filesystem::exists(folder))
        return;

    auto it = boost::filesystem::directory_iterator(folder);
    for(; it != boost::filesystem::end(it); ++it)
    {
        if(boost::filesystem::is_directory(it->path()))
            fillFilesList(it->path().string());
        else if(boost::filesystem::is_regular_file(it->path()) &&
                (Utils::File::extension(it->path().string()) == ".dcm" || Utils::File::extension(it->path().string()) == ""))
        {
            m_files.push_back(it->path().string());
        }
    }
}

void CDicomImageIO::sortFiles()
{
    vtkSmartPointer<vtkStringArray> files = vtkSmartPointer<vtkStringArray>::New();
    m_pDicomSorter = vtkSmartPointer<vtkDICOMFileSorter>::New();

    for(size_t i=0; i<m_files.size(); ++i)
        files->InsertNextValue(m_files[i]);

    m_pDicomSorter->SetInputFileNames(files);
    m_pDicomSorter->Update();
}

vtkSmartPointer<vtkImageData> CDicomImageIO::vtkImgData(const SubsetBounds &bounds)
{
    if(m_currentFile.empty() == false)
    {
        auto pImgData = vtkSmartPointer<vtkImageData>::New();
        auto pReader = vtkSmartPointer<vtkDICOMReader>::New();
        pReader->SetFileName(m_currentFile.c_str());
        pReader->Update();
        pImgData = pReader->GetOutput();
        return pImgData;
    }
    else
    {
        assert(m_pDicomSorter != nullptr);

        if(m_pDicomSorter->GetInputFileNames()->GetNumberOfValues() == 0)
            return nullptr;
        else
        {
            auto pImgData = vtkSmartPointer<vtkImageData>::New();
            auto pReader = vtkSmartPointer<vtkDICOMReader>::New();

            if(isValidSubsetBounds(bounds) == false)
            {
                //Get first serie index the firststudy
                int firstSerie = m_pDicomSorter->GetFirstSeriesForStudy(0);
                //Get all file names for the first serie
                vtkStringArray* files = m_pDicomSorter->GetFileNamesForSeries(firstSerie);
                //Set the first image path to the reader
                pReader->SetFileName(files->GetValue(0));
            }
            else
            {
                //Get first serie index for given study
                int firstSerie = m_pDicomSorter->GetFirstSeriesForStudy(static_cast<int>(Utils::Data::getDimensionBounds(bounds, DataDimension::STUDY).first));
                //Get all file names for given serie
                vtkStringArray* files = m_pDicomSorter->GetFileNamesForSeries(firstSerie + static_cast<int>(Utils::Data::getDimensionBounds(bounds, DataDimension::SERIE).first));
                //Set the given image path to the reader
                pReader->SetFileName(files->GetValue(Utils::Data::getDimensionBounds(bounds, DataDimension::IMAGE).first));
            }
            pReader->Update();
            pImgData = pReader->GetOutput();
            return pImgData;
        }
    }
}

vtkSmartPointer<vtkDICOMMetaData> CDicomImageIO::vtkMetadata(const SubsetBounds &bounds)
{
    if(m_currentFile.empty() == false)
    {
        auto pMetadata = vtkSmartPointer<vtkDICOMMetaData>::New();
        auto pReader = vtkSmartPointer<vtkDICOMReader>::New();
        pReader->SetFileName(m_currentFile.c_str());
        pReader->Update();
        pMetadata = pReader->GetMetaData();
        return pMetadata;
    }
    else
    {
        assert(m_pDicomSorter != nullptr);

        if(m_pDicomSorter->GetInputFileNames()->GetNumberOfValues() == 0)
            return nullptr;
        else
        {
            auto pMetadata = vtkSmartPointer<vtkDICOMMetaData>::New();
            auto pReader = vtkSmartPointer<vtkDICOMReader>::New();

            if(isValidSubsetBounds(bounds) == false)
            {
                //Get first serie index the firststudy
                int firstSerie = m_pDicomSorter->GetFirstSeriesForStudy(0);
                //Get all file names for the first serie
                vtkStringArray* files = m_pDicomSorter->GetFileNamesForSeries(firstSerie);
                //Set the first image path to the reader
                pReader->SetFileName(files->GetValue(0));
            }
            else
            {
                //Get first serie index for given study
                int firstSerie = m_pDicomSorter->GetFirstSeriesForStudy(static_cast<int>(Utils::Data::getDimensionBounds(bounds, DataDimension::STUDY).first));
                //Get all file names for given serie
                vtkStringArray* files = m_pDicomSorter->GetFileNamesForSeries(firstSerie + static_cast<int>(Utils::Data::getDimensionBounds(bounds, DataDimension::SERIE).first));
                //Set the given image path to the reader
                pReader->SetFileName(files->GetValue(Utils::Data::getDimensionBounds(bounds, DataDimension::IMAGE).first));
            }
            pReader->Update();
            pMetadata = pReader->GetMetaData();
            return pMetadata;
        }
    }
}

int CDicomImageIO::cvType(const vtkSmartPointer<vtkImageData>& pVtkImgData)
{
    assert(pVtkImgData != nullptr);
    int cvDepth;
    int scalarType = pVtkImgData->GetScalarType();

    switch(scalarType)
    {
        case VTK_BIT: cvDepth = CV_8S; break;               //Not managed in OpenCV
        case VTK_CHAR: cvDepth = CV_8S; break;              //Not managed in OpenCV
        case VTK_SIGNED_CHAR: cvDepth = CV_8S; break;
        case VTK_UNSIGNED_CHAR: cvDepth = CV_8U; break;
        case VTK_SHORT: cvDepth = CV_16S; break;
        case VTK_UNSIGNED_SHORT: cvDepth = CV_16U; break;
        case VTK_INT: cvDepth = CV_32S; break;
        case VTK_UNSIGNED_INT: cvDepth = CV_32S; break;     //Not managed in OpenCV
        case VTK_LONG: cvDepth = CV_32S; break;
        case VTK_UNSIGNED_LONG: cvDepth = CV_32S; break;    //Not managed in OpenCV
        case VTK_FLOAT: cvDepth = CV_32F; break;
        case VTK_DOUBLE: cvDepth = CV_64F; break;
        default: cvDepth = CV_8U; break;
    }

    int nbBand = pVtkImgData->GetNumberOfScalarComponents();
    if(nbBand > 4)
        nbBand = 1; //Indicate that a complete stack is stored inside data -> not managed

    return CV_MAKETYPE(cvDepth, nbBand);
}

void CDicomImageIO::vtkDisplay(const vtkSmartPointer<vtkImageData> &pImgData)
{
    vtkSmartPointer<vtkImageActor> imageActor = vtkSmartPointer<vtkImageActor>::New();
    imageActor->SetInputData(pImgData);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderer->AddActor(imageActor);
    renderer->SetBackground(0.1, 0.1, 0.2);
    renderWindow->Render();
    renderWindowInteractor->Start();
}

int CDicomImageIO::seriesCount(const SubsetBounds &bounds)
{
    assert(m_pDicomSorter != nullptr);

    int studyIndex;
    if(isValidSubsetBounds(bounds) == false)
        studyIndex = 0;
    else
    {
        studyIndex = (int)Utils::Data::getDimensionBounds(bounds, DataDimension::STUDY).first;
        if(studyIndex >= m_pDicomSorter->GetNumberOfStudies())
            return 0;
    }

    //Get first serie index for given study
    int firstSerie = m_pDicomSorter->GetFirstSeriesForStudy(studyIndex);
    int lastSerie = m_pDicomSorter->GetLastSeriesForStudy(studyIndex);
    return lastSerie - firstSerie + 1;
}
