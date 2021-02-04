#ifndef CDATASETIOWRAP_H
#define CDATASETIOWRAP_H

#include "PyDataProcessGlobal.h"
#include "IO/CDatasetIO.hpp"


class CDatasetIOWrap : public CDatasetIO, public wrapper<CDatasetIO>
{
    public:

        CDatasetIOWrap();
        CDatasetIOWrap(CDatasetIO::Format sourceFormat);

        std::vector<std::string>            getImagePaths() const override;
        std::vector<std::string>            default_getImagePaths() const;
        MapIntStr                           getCategories() const override;
        MapIntStr                           default_getCategories() const;
        int                                 getCategoryCount() const override;
        int                                 default_getCategoryCount() const;
        std::vector<ProxyGraphicsItemPtr>   getAnnotationGraphics(const std::string &imgPath) const override;
        std::vector<ProxyGraphicsItemPtr>   default_getAnnotationGraphics(const std::string &imgPath) const;
        std::string                         getMaskPath(const std::string& imgPath) const override;
        std::string                         default_getMaskPath(const std::string& imgPath) const;

        bool                                isDataAvailable() const override;
        bool                                default_isDataAvailable() const;

        void                                clearData() override;
        void                                default_clearData();

        void                                save(const std::string& path) override;
        void                                default_save(const std::string& path);
};

#endif // CDATASETIOWRAP_H
