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

#ifndef EXCEPTIONCODE_HPP
#define EXCEPTIONCODE_HPP

/** @file ExceptionCode.hpp */

namespace Ikomia
{
    /**
     * @enum DataIOExCode
     * @brief Exception codes for input/output operations
     */
    enum DataIOExCode
    {
        FILE_NOT_EXISTS,        /**< File does not exist */
        VIDEO_WRONG_IMG_NUMBERS /**< For image sequence: mismatch between the name pattern (digits count) and the number of image */
    };

    /**
     * @enum CoreExCode
     * @brief Core exception codes
     */
    enum CoreExCode
    {
        UNKNOWN,                /**< Unknown exception */
        STRUCTURE_OVERFLOW,     /**< Maximum data structure reached */
        INDEX_OVERFLOW,         /**< Index out of bounds */
        INVALID_SIZE,           /**< Invalid structure size (ex:image) */
        INVALID_MODEL_INDEX,    /**< Internal code */
        INVALID_DIMENSION,      /**< Invalid dataset dimensions */
        INVALID_PARAMETER,      /**< Generic code for parameters errors */
        INVALID_IMAGE,          /**< Empty or invalid image */
        INVALID_CONNECTION,     /**< Protocol: invalid connection between 2 tasks */
        INVALID_FILE,           /**< Invalid file (format, extension, size...) */
        INVALID_USAGE,          /**< Invalid function usage (insatisfied prerequisites) */
        INVALID_VIDEO_WRITER,   /**< Invalid parameters for video writer */
        OUT_OF_MEMORY,          /**< Maximum memory load reached */
        CREATE_FAILED,          /**< Allocation failed */
        NULL_POINTER,           /**< Null pointer exceptio */
        INVALID_JSON_FORMAT,    /**< Invalid JSON format */
        PROCESS_CANCELLED,      /**< Process does not end correctly */
        PYTHON_EXCEPTION,       /**< Python error */
        TIMEOUT_REACHED,        /**< Timeout reached */
        NOT_IMPLEMENTED         /**< Internal code */
    };

    /**
     * @enum DatabaseExCode
     * @brief Database exception codes
     */
    enum DatabaseExCode
    {
        INVALID_DB_CONNECTION,  /**< Invalid connection to remote DB */
        INVALID_QUERY,          /**< Invalid SQL query */
        INVALID_ID              /**< Invalid record primary key */
    };

    /// @cond INTERNAL
    enum AppExCode
    {
        INVALID
    };
    /// @endcond

    /**
     * @enum OpenGLExCode
     * @brief OpenGL exception codes
     */
    enum OpenGLExCode
    {
        CONTEXT_ERROR,          /**< Error on context init*/
        SHADER_COMPILE_ERROR,   /**< Compilation error on GLSL shader */
        SHADER_PROGRAM_ERROR,   /**< Program error on GLSL shader */
        SHADER_LINK_ERROR,      /**< Link error on GLSL shader */
        SHADER_BIND_ERROR       /**< Bind error on GLSL shader */
    };

    /**
     * @enum HttpExCode
     * @brief Http exception codes
     */
    enum HttpExCode
    {
        INVALID_URL             /**< Invalid URL (format or unreachable) */
    };
}

#endif // EXCEPTIONCODE_HPP
