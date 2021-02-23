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

#ifndef COREDEFINE_HPP
#define COREDEFINE_HPP

#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <QString>
#include "Graphics/CPoint.hpp"

namespace Ikomia
{
    /**
     * @brief
     *
     */
    enum class DataDimension : size_t
    {
        NONE,
        X,
        Y,
        Z,
        IMAGE,
        TIME,
        POSITION,
        VOLUME,
        MODALITY,
        STUDY,
        SERIE
    };

    /**
     * @enum Data
     * @brief Input/output data types
     */
    enum class IODataType : size_t
    {
        IMAGE,                  /**< Image data type */
        VIDEO,                  /**< Video from file (AVI, MPEG...) */
        VOLUME,                 /**< Volume data type */
        LIVE_STREAM,            /**< Video from stream (camera) */
        INPUT_GRAPHICS,         /**< Graphics data type: graphics layer with graphics items (ellipse, rectangle, text...) */
        OUTPUT_GRAPHICS,        /**< Graphics data type: graphics layer with graphics items (ellipse, rectangle, text...) */
        BLOB_VALUES,            /**< Numeric values from measure (surface, diameter...) computed on connected component */
        NUMERIC_VALUES,         /**< Generic numeric values */
        DESCRIPTORS,            /**< Image descriptors (used for classification, registration...) */
        WIDGET,                 /**< User-defined widget */
        FOLDER,                 /**< Ikomia project folder: may contain various data type */
        FOLDER_PATH,            /**< Folder path */
        FILE_PATH,              /**< File path */
        DNN_DATASET,            /**< Dataset used for deep learning, composed with image and annotations */
        VOLUME_BINARY,          /**< Binary volume data type: 8bits single channel */
        VOLUME_LABEL,           /**< Label volume data type: single channel, 1 graylevel per connected component */
        IMAGE_BINARY,           /**< Binary image data type: 8bits single channel */
        IMAGE_LABEL,            /**< Label image data type: single channel, 1 graylevel per connected component */
        VIDEO_BINARY,           /**< Binary video from file (AVI, MPEG...): 8bits single channel */
        VIDEO_LABEL,            /**< Label video from file (AVI, MPEG...): single channel, 1 graylevel per connected component */
        LIVE_STREAM_BINARY,     /**< Binary video from stream (camera): 8bits single channel */
        LIVE_STREAM_LABEL,      /**< Label video from stream (camera): single channel, 1 graylevel per connected component */
        ARRAY,                  /**< Multi-dimensional array */
        NONE                    /**< Unknown data type */
    };

    // Enum class mandatory to avoir name conflict on Windows...
    enum class GraphicsItem : size_t
    {
        LAYER,
        POINT,
        ELLIPSE,
        RECTANGLE,
        POLYGON,
        COMPLEX_POLYGON,
        POLYLINE,
        TEXT
    };

    /**
     * @enum GraphicsShape
     * @brief Graphics item shape values
     */
    enum class GraphicsShape : size_t
    {
        SELECTION,          /**< Rectangle selection area */
        POINT,              /**< Point item */
        ELLIPSE,            /**< Ellipse or circle item */
        RECTANGLE,          /**< Rectangle or square item */
        POLYGON,            /**< Polygon item */
        FREEHAND_POLYGON,   /**< Polygon item (free hand) */
        LINE,               /**< Line item (2 points) */
        POLYLINE,           /**< Polyline item */
        FREEHAND_POLYLINE,  /**< Polyline item (free hand) */
        TEXT                /**< Text item */
    };

    enum class DataFileFormat : size_t
    {
        TXT, JSON, XML, YAML, CSV,
        BMP, JPG, JP2, PNG, TIF, WEBP,
        AVI, MPEG
    };

    /**
     * @brief
     *
     */
    using Dimension = std::pair<DataDimension,size_t>;
    /**
     * @brief
     *
     */
    using Dimensions = std::vector<Dimension>;
    /**
     * @brief
     *
     */
    using Bounds = std::pair<size_t,size_t>;
    /**
     * @brief
     *
     */
    using DimensionBounds = std::pair<DataDimension, Bounds>;
    /**
     * @brief
     *
     */
    using SubsetBounds = std::vector<DimensionBounds>;
    /**
     * @brief
     *
     */
    using DimensionIndices = Dimensions;

    constexpr double _pi = 3.14159265358979323846;

    using MapIntStr = std::map<int, std::string>;
    using UMapString = std::unordered_map<std::string, std::string>;
    using PairString = std::pair<std::string, std::string>;
    using VectorString = std::vector<std::string>;
    using VectorPairString = std::vector<std::pair<std::string, std::string>>;
    using VectorPairQString = std::vector<std::pair<QString, QString>>;
    using CColor = std::vector<int>;
}

using namespace Ikomia;

#endif // COREDEFINE_HPP
