// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef CDATASETIO_H
#define CDATASETIO_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include "Main/CoreGlobal.hpp"
#include "Protocol/CProtocolTaskIO.h"
#include "Graphics/CGraphicsItem.hpp"

class CDatasetIO: public CProtocolTaskIO
{
    public:

        enum Format {OTHER, COCO, PASCAL_VOC, YOLO, VIA};

        CDatasetIO() : CProtocolTaskIO(IODataType::DNN_DATASET)
        {
            m_srcFormat = Format::OTHER;
            m_description = QObject::tr("Annotated image dataset in Ikomia format (for Deep Learning).").toStdString();
            m_saveFormat = DataFileFormat::JSON;
        }
        CDatasetIO(Format srcFormat) : CProtocolTaskIO(IODataType::DNN_DATASET)
        {
            m_srcFormat = srcFormat;
            m_description = QObject::tr("Annotated image dataset in Ikomia format (for Deep Learning).").toStdString();
            m_saveFormat = DataFileFormat::JSON;
        }

        virtual ~CDatasetIO() = default;

        Format                                      getSourceFormat() const
        {
            return m_srcFormat;
        }
        virtual std::vector<std::string>            getImagePaths() const
        {
            return std::vector<std::string>();
        }
        virtual MapIntStr                           getCategories() const
        {
            return MapIntStr();
        }
        virtual int                                 getCategoryCount() const
        {
            return 0;
        }
        virtual std::vector<ProxyGraphicsItemPtr>   getAnnotationGraphics(const std::string &imgPath) const
        {
            Q_UNUSED(imgPath);
            return std::vector<ProxyGraphicsItemPtr>();
        }
        virtual std::string                         getMaskPath(const std::string& imgPath) const
        {
            Q_UNUSED(imgPath);
            return "";
        }
        QJsonDocument                               getJsonDocument() const
        {
            return m_json;
        }

        bool                                        isDataAvailable() const override
        {
            return false;
        }
        bool                                        isAutoInput() const override
        {
            return true;
        }

        void                                        clearData()
        {
        }

        void                                        save(const std::string& path) override
        {
            if(m_json.isNull())
                return;

            QFile jsonFile(QString::fromStdString(path));
            if(jsonFile.open(QFile::WriteOnly | QFile::Text))
                jsonFile.write(m_json.toJson());
        }

        bool                                        load(const std::string& path)
        {
            QFile jsonFile(QString::fromStdString(path));
            if(jsonFile.open(QFile::ReadOnly | QFile::Text) == false)
                return false;

            m_json = QJsonDocument::fromJson(jsonFile.readAll());
            jsonFile.close();
            return true;
        }

    private:

        Format          m_srcFormat;
        QJsonDocument   m_json;
};

#endif // CDATASETIO_H
