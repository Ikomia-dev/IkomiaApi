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

#ifndef DATAUTILS_HPP
#define DATAUTILS_HPP

#include <QObject>
#include <QDir>
#include <QSqlQuery>
#include <QPainter>
#include <QRandomGenerator>
#include "UtilsTools.hpp"
#include "Main/CoreDefine.hpp"
#include "Protocol/CProtocolTaskIO.h"
#include "Data/CMat.hpp"
#include "Data/CMeasure.h"
#include "Graphics/CGraphicsLayer.h"
#include "Graphics/CGraphicsLayer.h"
#include "Graphics/CGraphicsPoint.h"
#include "Graphics/CGraphicsEllipse.h"
#include "Graphics/CGraphicsPolygon.h"
#include "Graphics/CGraphicsPolyline.h"
#include "Graphics/CGraphicsComplexPolygon.h"
#include "Graphics/CGraphicsRectangle.h"
#include "Graphics/CGraphicsText.h"

namespace Ikomia
{
    namespace Utils
    {
        inline std::string to_string(const cv::KeyPoint& kpt)
        {
            return "angle:" + std::to_string(kpt.angle) +
                    "; class:" + std::to_string(kpt.class_id) +
                    "; x:" + std::to_string(kpt.pt.x) +
                    "; y:" + std::to_string(kpt.pt.y) +
                    "; size:" + std::to_string(kpt.size);
        }

        template<typename T>
        inline std::string to_string(const cv::Point_<T>& pt)
        {
            return "x:" + std::to_string(pt.x) +
                    "; y:" + std::to_string(pt.y);
        }

        namespace Data
        {
            inline std::string          getDimensionName(DataDimension dim)
            {
                switch(dim)
                {
                    case DataDimension::IMAGE:
                        return "Image";
                    case DataDimension::MODALITY:
                        return QObject::tr("Modality").toStdString();
                    case DataDimension::POSITION:
                        return QObject::tr("Position").toStdString();
                    case DataDimension::TIME:
                        return QObject::tr("Time").toStdString();
                    case DataDimension::VOLUME:
                        return QObject::tr("Volume").toStdString();
                    case DataDimension::STUDY:
                        return QObject::tr("Study").toStdString();
                    case DataDimension::SERIE:
                        return QObject::tr("Serie").toStdString();
                    default:
                        return QObject::tr("Dimension").toStdString();;
                }
            }

            inline size_t               getDimensionSize(Dimensions dims, DataDimension dim)
            {
                for(size_t i=0; i<dims.size(); ++i)
                {
                    if(dims[i].first == dim)
                        return dims[i].second;
                }
                return 0;
            }

            inline Bounds               getDimensionBounds(const SubsetBounds& bounds, DataDimension dim)
            {
                for(size_t i=0; i<bounds.size(); ++i)
                {
                    if(bounds[i].first == dim)
                        return bounds[i].second;
                }
                return Bounds(-1, -1);
            }

            inline size_t               getBoundsSize(const Bounds& bounds)
            {
                return bounds.second - bounds.first + 1;
            }

            inline size_t               getSubsetBoundsSize(const SubsetBounds& bounds)
            {
                if(bounds.size() == 0)
                    return 0;

                size_t nb = 1;
                for(size_t i=0; i<bounds.size(); ++i)
                    nb *= getBoundsSize(bounds[i].second);

                return nb;
            }

            inline DimensionIndices     subsetBoundsToIndices(const SubsetBounds& bounds)
            {
                DimensionIndices indices;
                for(size_t i=0; i<bounds.size(); ++i)
                    indices.push_back(std::make_pair(bounds[i].first, bounds[i].second.first));
                return indices;
            }

            inline DimensionIndices     allocateDimensionIndices(Dimensions dims)
            {
                DimensionIndices indices;
                for(size_t i=0; i<dims.size(); ++i)
                    indices.push_back(std::make_pair(dims[i].first, 0));
                return indices;
            }

            inline void                 setDimensionIndex(DimensionIndices& indices, size_t dimIndex, size_t index)
            {
                if(dimIndex < indices.size())
                    indices[dimIndex].second = index;
            }

            inline void                 setSubsetBounds(SubsetBounds& bounds, DataDimension dim, size_t dimIndexFirst, size_t dimIndexLast)
            {
                for(size_t i=0; i<bounds.size(); ++i)
                {
                    if(bounds[i].first == dim)
                    {
                        bounds[i].second = {dimIndexFirst, dimIndexLast};
                    }
                }
            }

            inline std::string          getFileFormatExtension(DataFileFormat format)
            {
                switch(format)
                {
                    case DataFileFormat::NONE: return "";
                    case DataFileFormat::TXT: return ".txt";
                    case DataFileFormat::JSON: return ".json";
                    case DataFileFormat::XML: return ".xml";
                    case DataFileFormat::YAML: return ".yml";
                    case DataFileFormat::CSV: return ".csv";
                    case DataFileFormat::BMP: return ".bmp";
                    case DataFileFormat::JPG: return ".jpg";
                    case DataFileFormat::JP2: return ".jp2";
                    case DataFileFormat::PNG: return ".png";
                    case DataFileFormat::TIF: return ".tif";
                    case DataFileFormat::WEBP: return ".webp";
                    case DataFileFormat::AVI: return ".avi";
                    case DataFileFormat::MPEG: return ".json";
                }
                return "";
            }
        }

        namespace Database
        {
            inline QString  getMainPath()
            {
                QDir dir;
                QString path = IkomiaApp::getQAppFolder();
                dir.mkpath(path);
                path += "/Ikomia.pcl";
                return path;
            }

            inline QString  getMainConnectionName()
            {
                return "MainConnection";
            }
        }

        namespace Graphics
        {
            inline void             paintSelectedVertex(QPainter *painter, const QPointF &vertex, const qreal lineSize)
            {
                assert(painter);
                const qreal halfSize = 2.0 * lineSize;
                const qreal size = 2.0 * halfSize;

                QPen pen(QColor(0, 0, 0, 255));
                pen.setCosmetic(true);
                painter->setPen(pen);
                painter->setBrush(QColor(255, 255, 255, 255));
                painter->drawRect(QRectF(vertex.x() - halfSize, vertex.y() - halfSize, size, size));
            }
            inline qreal            getSelectionMargin(const qreal lineSize)
            {
                return 2.0 * lineSize;
            }
            inline QColor           toQColor(const CColor& color)
            {
                if(color.size() == 3)
                    return QColor(color[0], color[1], color[2]);
                else if(color.size() == 4)
                    return QColor(color[0], color[1], color[2], color[3]);
                else
                    return QColor();
            }
            inline CColor           toCColor(const QColor& color)
            {
                CColor ccolor = {color.red(), color.green(), color.blue(), color.alpha()};
                return ccolor;
            }
            inline QColor           getRandomQColor()
            {
                return QColor::fromRgb(QRandomGenerator::global()->generate());
            }
            inline CColor           getRandomCColor()
            {
                return toCColor(QColor::fromRgb(QRandomGenerator::global()->generate()));
            }
            inline CGraphicsLayer*  createLayer(const QString& name, const std::vector<ProxyGraphicsItemPtr>& items, const GraphicsContextPtr &globalContext)
            {
                //Ugly switch case but we can't do polymorphism to allow Python bindings
                //We don't want to manage Qt-based pointer into CProxyGraphicsItem based object
                auto pLayer = new CGraphicsLayer(name);
                for(size_t i=0; i<items.size(); ++i)
                {
                    GraphicsItem type = static_cast<GraphicsItem>(items[i]->getType());
                    switch(type)
                    {
                        case GraphicsItem::LAYER: break;
                        case GraphicsItem::RECTANGLE:
                        {
                            auto pItem = new CGraphicsRectangle(globalContext, std::static_pointer_cast<CProxyGraphicsRect>(items[i]), pLayer);
                            Q_UNUSED(pItem);
                            break;
                        }
                        case GraphicsItem::ELLIPSE:
                        {
                            auto pItem = new CGraphicsEllipse(globalContext, std::static_pointer_cast<CProxyGraphicsEllipse>(items[i]), pLayer);
                            Q_UNUSED(pItem);
                            break;
                        }
                        case GraphicsItem::POINT:
                        {
                            auto pItem = new CGraphicsPoint(globalContext, std::static_pointer_cast<CProxyGraphicsPoint>(items[i]), pLayer);
                            Q_UNUSED(pItem);
                            break;
                        }
                        case GraphicsItem::POLYGON:
                        {
                            auto pItem = new CGraphicsPolygon(globalContext, std::static_pointer_cast<CProxyGraphicsPolygon>(items[i]), pLayer);
                            Q_UNUSED(pItem);
                            break;
                        }
                        case GraphicsItem::POLYLINE:
                        {
                            auto pItem = new CGraphicsPolyline(globalContext, std::static_pointer_cast<CProxyGraphicsPolyline>(items[i]), pLayer);
                            Q_UNUSED(pItem);
                            break;
                        }
                        case GraphicsItem::COMPLEX_POLYGON:
                        {
                            auto pItem = new CGraphicsComplexPolygon(globalContext, std::static_pointer_cast<CProxyGraphicsComplexPoly>(items[i]), pLayer);
                            Q_UNUSED(pItem);
                            break;
                        }
                        case GraphicsItem::TEXT:
                        {
                            auto pItem = new CGraphicsText(globalContext, std::static_pointer_cast<CProxyGraphicsText>(items[i]), pLayer);
                            Q_UNUSED(pItem);
                            break;
                        }
                    }
                }
                return pLayer;
            }
        }

        namespace Font
        {
            inline double   getQtToOcvFontScaleFactor(int size)
            {
                //OpenCV scale factor of 1.0 corresponds approximatively to Qt font size of 25
                return (double)size / 25.0;
            }
        }

        namespace Protocol
        {
            inline QString  getIODataName(IODataType dataType)
            {
                switch(dataType)
                {
                    case IODataType::NONE:
                        return QObject::tr("none");
                    case IODataType::IMAGE:
                        return QObject::tr("image");
                    case IODataType::IMAGE_BINARY:
                        return QObject::tr("binary image");
                    case IODataType::IMAGE_LABEL:
                        return QObject::tr("label image");
                    case IODataType::VOLUME:
                        return QObject::tr("volume");
                    case IODataType::VOLUME_BINARY:
                        return QObject::tr("binary volume");
                    case IODataType::VOLUME_LABEL:
                        return QObject::tr("label volume");
                    case IODataType::INPUT_GRAPHICS:
                    case IODataType::OUTPUT_GRAPHICS:
                        return QObject::tr("graphics");
                    case IODataType::BLOB_VALUES:
                        return QObject::tr("blob values");
                    case IODataType::NUMERIC_VALUES:
                        return QObject::tr("numeric values");
                    case IODataType::VIDEO:
                        return QObject::tr("video");
                    case IODataType::VIDEO_BINARY:
                        return QObject::tr("binary video");
                    case IODataType::VIDEO_LABEL:
                        return QObject::tr("label Video");
                    case IODataType::LIVE_STREAM:
                        return QObject::tr("live stream");
                    case IODataType::LIVE_STREAM_BINARY:
                        return QObject::tr("binary live stream");
                    case IODataType::LIVE_STREAM_LABEL:
                        return QObject::tr("label live stream");
                    case IODataType::WIDGET:
                        return QObject::tr("custom result view");
                    case IODataType::DESCRIPTORS:
                        return QObject::tr("descriptors");
                    case IODataType::PROJECT_FOLDER:
                        return QObject::tr("folder");
                    case IODataType::FOLDER_PATH:
                        return QObject::tr("folder path");
                    case IODataType::FILE_PATH:
                        return QObject::tr("file path");
                    case IODataType::DNN_DATASET:
                        return QObject::tr("deep learning dataset");
                    case IODataType::ARRAY:
                        return QObject::tr("multi-dimensional array");
                }
                return QString();
            }

            inline bool     isConvertibleIO(ProtocolTaskIOPtr from, ProtocolTaskIOPtr to)
            {
                if(from == nullptr || to == nullptr)
                    return true;

                auto nameFrom = CProtocolTaskIO::getClassName(from->getDataType());
                auto nameTo = CProtocolTaskIO::getClassName(to->getDataType());

                return (nameFrom == nameTo ||
                        (nameFrom == "CImageProcessIO" && nameTo == "CVideoProcessIO") ||
                        (nameFrom == "CVideoProcessIO" && nameTo == "CImageProcessIO") ||
                        (nameFrom == "CPathIO" && nameTo == "CImageProcessIO") ||
                        (nameFrom == "CPathIO" && nameTo == "CVideoProcessIO"));
            }

            /**
             * @brief Checks if two data types can be connected.
             * @param srcData: source data type ::IODataType.
             * @param targetData: target data type ::IODataType.
             * @return True if connection is possible, False otherwise.
             */
            inline bool isIODataCompatible(const IODataType &srcData, const IODataType &targetData)
            {
                if(srcData == IODataType::NONE || targetData == IODataType::NONE)
                    return false;
                else if(srcData == targetData)
                    return true;
                else if(srcData == IODataType::INPUT_GRAPHICS)
                    return targetData == IODataType::OUTPUT_GRAPHICS;
                else if(srcData == IODataType::OUTPUT_GRAPHICS)
                    return targetData == IODataType::INPUT_GRAPHICS;
                else if(srcData == IODataType::IMAGE_BINARY)
                {
                    return (targetData == IODataType::IMAGE ||
                            targetData == IODataType::IMAGE_LABEL);
                }
                else if(srcData == IODataType::IMAGE_LABEL)
                {
                    return targetData == IODataType::IMAGE;
                }
                else if(srcData == IODataType::VOLUME)
                {
                    return targetData == IODataType::IMAGE;
                }
                else if(srcData == IODataType::VOLUME_BINARY)
                {
                    return (targetData == IODataType::VOLUME ||
                            targetData == IODataType::VOLUME_LABEL ||
                            targetData == IODataType::IMAGE ||
                            targetData ==  IODataType::IMAGE_BINARY ||
                            targetData ==  IODataType::IMAGE_LABEL);
                }
                else if(srcData == IODataType::VOLUME_LABEL)
                {
                    return (targetData == IODataType::IMAGE ||
                            targetData == IODataType::IMAGE_LABEL);
                }
                else if(srcData == IODataType::VIDEO)
                {
                    return targetData == IODataType::IMAGE;
                }
                else if(srcData == IODataType::VIDEO_BINARY)
                {
                    return (targetData == IODataType::VIDEO ||
                            targetData == IODataType::VIDEO_LABEL ||
                            targetData == IODataType::IMAGE ||
                            targetData == IODataType::IMAGE_BINARY ||
                            targetData == IODataType::IMAGE_LABEL);
                }
                else if(srcData == IODataType::VIDEO_LABEL)
                {
                    return (targetData == IODataType::VIDEO ||
                            targetData == IODataType::IMAGE ||
                            targetData == IODataType::IMAGE_LABEL);
                }
                else if(srcData == IODataType::LIVE_STREAM)
                {
                    return (targetData == IODataType::IMAGE ||
                            targetData == IODataType::VIDEO);
                }
                else if(srcData == IODataType::LIVE_STREAM_BINARY)
                {
                    return targetData == IODataType::LIVE_STREAM ||
                            targetData == IODataType::LIVE_STREAM_LABEL ||
                            targetData == IODataType::IMAGE ||
                            targetData == IODataType::IMAGE_BINARY ||
                            targetData == IODataType::IMAGE_LABEL ||
                            targetData == IODataType::VIDEO ||
                            targetData == IODataType::VIDEO_BINARY ||
                            targetData == IODataType::VIDEO_LABEL;
                }
                else if(srcData == IODataType::LIVE_STREAM_LABEL)
                {
                    return (targetData == IODataType::LIVE_STREAM ||
                            targetData == IODataType::VIDEO ||
                            targetData == IODataType::VIDEO_LABEL ||
                            targetData == IODataType::IMAGE ||
                            targetData == IODataType::IMAGE_LABEL);
                }
                else if(srcData == IODataType::PROJECT_FOLDER)
                {
                    return targetData == IODataType::IMAGE ||
                            targetData == IODataType::IMAGE_BINARY ||
                            targetData == IODataType::IMAGE_LABEL ||
                            targetData == IODataType::VIDEO ||
                            targetData == IODataType::VIDEO_BINARY ||
                            targetData == IODataType::VIDEO_LABEL ||
                            targetData == IODataType::PROJECT_FOLDER ||
                            targetData == IODataType::FOLDER_PATH;
                }
                else
                    return false;
            }
        }

        namespace Image
        {
            /**
            * @brief Gets the maximum theoretical pixel value deduced from bits per pixel.
            * @param img: image.
            * @return maximum pixel value.
            */
            inline double   getMaxValue(const CMat& img)
            {
                double value = 0;
                int depth = img.depth();

                switch(depth)
                {
                    case CV_8S: value = 127; break;
                    case CV_8U: value = 255; break;
                    case CV_16S: value = 32767; break;
                    case CV_16U: value = 65535; break;
                    case CV_32S: value = 2147483647; break;
                    case CV_32F: value = 1.0; break;
                    case CV_64F: value = 1.0; break;
                }
                return value;
            }
        }
    }
}

using namespace Ikomia;

#endif // DATAUTILS_HPP
