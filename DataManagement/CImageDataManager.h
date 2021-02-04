#ifndef CIMAGEDATAMANAGER_H
#define CIMAGEDATAMANAGER_H

#include "CDataManager.hpp"
#include "CDataIO.hpp"
#include "DataManagementGlobal.hpp"

//Register log category
Q_DECLARE_LOGGING_CATEGORY(LogImageDataManager)

using CImageIO = CDataIO<CDataImageIO, CMat>;
using CImageIOPtr = std::shared_ptr<CDataIO<CDataImageIO, CMat>>;
using CDataImageInfoPtr = std::shared_ptr<CDataImageInfo>;

class DATAMANAGEMENTSHARED_EXPORT CImageDataManager : public CDataManager<CMat>
{
    public:

        CImageDataManager();

        virtual ~CImageDataManager();

        virtual void        loadData(CDataset<CMat>& dataset);
        virtual void        loadData(CDataset<CMat>& dataset, const SubsetBounds& subsetBounds);
        virtual void        clearData();

        bool                isInMemory(CDataset<CMat>& dataset, const SubsetBounds& subsetBounds);

    private:

        CImageIOPtr         getImageIO(const std::string& fileName);

        void                fillDataset(CDataset<CMat>& dataset, const SubsetBounds &subsetBounds, const CArrayDataInfo& arrayDataInfo);

        void                cropDataset(CDataset<CMat>& dataset, const SubsetBounds &subsetBounds);

        Dimensions          dimensions(const SubsetBounds &bounds);
        Dimensions          dimensions(const SubsetBounds &bounds, DataDimension groupBy);
        SubsetBounds        dimensionBounds(const SubsetBounds& bounds, DataDimension dimension);

        bool                checkMemory(const CArrayDataInfo &arrayDataInfo);

    private:

        using CMapFileImageIO = std::unordered_map<std::string, CImageIOPtr>;

        CMapFileImageIO     m_mapImageIO;
};

#endif // CIMAGEDATAMANAGER_H
