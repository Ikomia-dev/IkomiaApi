#ifndef CVIDEODATAMANAGER_H
#define CVIDEODATAMANAGER_H

#include "CDataManager.hpp"
#include "DataManagementGlobal.hpp"
#include "CDataIO.hpp"

//Register log category
Q_DECLARE_LOGGING_CATEGORY(LogVideoDataManager)

using CVideoIO = CDataIO<CDataVideoIO, CMat>;
using CVideoIOPtr = std::shared_ptr<CDataIO<CDataVideoIO, CMat>>;
using CDataVideoInfoPtr = std::shared_ptr<CDataVideoInfo>;

class DATAMANAGEMENTSHARED_EXPORT CVideoDataManager : public CDataManager<CMat>
{
    public:

        CVideoDataManager();

        virtual ~CVideoDataManager();

        virtual void        loadData(CDataset<CMat>& dataset);
        virtual void        loadData(CDataset<CMat>& dataset, const SubsetBounds& subsetBounds);
        virtual void        clearData();

        bool                isInMemory(CDataset<CMat>& dataset, const SubsetBounds& subsetBounds);

        void                setLiveMode(bool bLive);
        void                setCurrentVideoIO(CDataset<CMat>& dataset);

        CDataVideoInfoPtr   getDataVideoInfoPtr();

        CMat                playVideo(CDataset<CMat> &dataset);
        CMat                playVideo(CDataset<CMat> &dataset, const SubsetBounds& subsetBounds);

        void                writeVideo(const CMat& image, const std::string& path);

        void                stopWriteVideo();
        void                stopReadVideo();

        void                waitWriteFinished();

        void                closeCamera();

    private:

        CVideoIOPtr         getVideoIO(const std::string& fileName);

        void                fillDataset(CDataset<CMat>& dataset, const SubsetBounds &subsetBounds, const CArrayDataInfo& arrayDataInfo);

        void                cropDataset(CDataset<CMat>& dataset, const SubsetBounds &subsetBounds);

        Dimensions          dimensions(const SubsetBounds &bounds);
        Dimensions          dimensions(const SubsetBounds &bounds, DataDimension groupBy);
        SubsetBounds        dimensionBounds(const SubsetBounds& bounds, DataDimension dimension);

        bool                checkMemory(const CArrayDataInfo &arrayDataInfo);
        bool                isCamOrIPStreamLinux(const std::string& filename);

    private:

        using CMapFileVideoIO = std::unordered_map<std::string, CVideoIOPtr>;

        CMapFileVideoIO     m_mapVideoIO;
        CVideoIOPtr         m_pCurrentVideoIO = nullptr;
        bool                m_bLive = false;
};

#endif // CVIDEODATAMANAGER_H
