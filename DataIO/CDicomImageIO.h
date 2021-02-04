#ifndef CDICOMIMAGEIO_H
#define CDICOMIMAGEIO_H

#include "CVirtualImageIO.h"
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkDICOMMetaData.h>
#include <vtkDICOMFileSorter.h>
#include "DataIOGlobal.hpp"

class DATAIOSHARED_EXPORT CDicomImageIO : public CVirtualImageIO
{
    public:

        CDicomImageIO(const std::string &path);
        virtual ~CDicomImageIO();

        VectorString                        fileNames(const SubsetBounds& bounds) override;

        Dimensions                          dimensions() override;
        Dimensions                          dimensions(const SubsetBounds& bounds) override;

        CDataInfoPtr                        dataInfo() override;
        CDataInfoPtr                        dataInfo(const SubsetBounds& bounds) override;

        CMat                                read() override;
        CMat                                read(const SubsetBounds& bounds) override;

    private:

        bool                                isValidSubsetBounds(const SubsetBounds& bounds);

        void                                fillFilesList(const std::string& folder);

        void                                sortFiles();

        vtkSmartPointer<vtkImageData>       vtkImgData(const SubsetBounds& bounds);

        vtkSmartPointer<vtkDICOMMetaData>	vtkMetadata(const SubsetBounds& bounds);

        int                                 cvType(const vtkSmartPointer<vtkImageData>& pVtkImgData);

        void                                vtkDisplay(const vtkSmartPointer<vtkImageData>& pImgData);

        int                                 seriesCount(const SubsetBounds &bounds);

    private:

        using MapFileDicomStruct = std::unordered_map<std::string, std::pair<int, int>>;

        //bool                                m_bSingleFile;
        std::vector<std::string>            m_files;
        std::string                         m_currentFile;
        vtkSmartPointer<vtkDICOMFileSorter> m_pDicomSorter;
        std::mutex                          m_mutex;
};

#endif // CDICOMIMAGEIO_H
