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

#ifndef CDATAIO_H
#define CDATAIO_H

#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include "DataIOGlobal.hpp"
#include "CDataImageIO.h"
#include "CDataVideoIO.h"

/**
 * @brief
 *
 */
class _CDataIO
{
    public:

        /**
         * @brief
         *
         * @param fileName
         * @return DataType
         */
        static IODataType dataType(const std::string& fileName)
        {
            return CDataImageIO::dataType(fileName);
        }

        /**
         * @brief
         *
         * @param fileName
         * @return DataType
         */
        static Dimensions dimensions(const std::string& fileName)
        {
            switch(dataType(fileName))
            {
                case IODataType::IMAGE:
                    return CDataImageIO::dimensions(fileName);
                default:
                    return Dimensions();
            }
            return Dimensions();
        }
};

/**
 * @brief
 *
 */
template <class _ClassIO, class _DataType>
class CDataIO : public _CDataIO
{
    //public methods
    public:

        //Constructors
        CDataIO(bool bNoFileStream, const std::string& fileName)
        {
            m_bNoFileStream = bNoFileStream;
            m_fileName = fileName;
            m_pDataIO = std::make_unique<_ClassIO>(fileName);
        }
        CDataIO(const std::string& fileName)
        {
            m_fileName = fileName;
            m_pDataIO = std::make_unique<_ClassIO>(fileName);
        }
        CDataIO(const CDataIO<_ClassIO,_DataType>& io)
        {
            m_fileName = io.m_fileName;
            m_pDataIO = nullptr;

            if(io.m_pDataIO)
                m_pDataIO = std::make_unique<_ClassIO>(*io.m_pDataIO);
        }
        CDataIO(CDataIO<_ClassIO,_DataType>&& io)
        {
            m_fileName = std::move(io.m_fileName);
            m_pDataIO = std::make_unique(std::move(io.m_pDataIO));
        }

        //Operators
        CDataIO<_ClassIO,_DataType>&    operator=(const CDataIO<_ClassIO,_DataType>& io)
        {
            m_fileName = io.m_fileName;

            if(io.m_pDataIO)
                m_pDataIO.reset(new _ClassIO(*io.m_pDataIO));
            else
                m_pDataIO.reset(nullptr);

            return *this;
        }
        CDataIO<_ClassIO,_DataType>&    operator=(CDataIO<_ClassIO,_DataType>&& io)
        {
            m_fileName = std::move(io.m_fileName);
            m_pDataIO = std::move(io.m_pDataIO);
            return *this;
        }

        VectorString                    fileNames(const SubsetBounds& bounds)
        {
            if(m_pDataIO)
                return m_pDataIO->fileNames(bounds);
            else
                return std::vector<std::string>();
        }

        Dimensions                      dimensions()
        {
            if(m_pDataIO)
                return m_pDataIO->dimensions();
            else
                return Dimensions();
        }
        Dimensions                      dimensions(const SubsetBounds& bounds)
        {
            if(m_pDataIO)
                return m_pDataIO->dimensions(bounds);
            else
                return Dimensions();
        }

        CDataInfoPtr                    dataInfo()
        {
            if(m_pDataIO)
                return m_pDataIO->dataInfo();
            else
                return nullptr;
        }
        CDataInfoPtr                    dataInfo(const SubsetBounds& bounds)
        {
            if(m_pDataIO)
                return m_pDataIO->dataInfo(bounds);
            else
                return nullptr;
        }

        _DataType                       read()
        {
            if(m_bNoFileStream == false && Utils::File::isFileExist(m_fileName) == false)
                throw CException(DataIOExCode::FILE_NOT_EXISTS, "File not exists", __func__, __FILE__, __LINE__);

            if(m_pDataIO)
                return m_pDataIO->read();
            else
                return _DataType();
        }
        _DataType                       read(const SubsetBounds& subset)
        {
            if(m_bNoFileStream == false && Utils::File::isFileExist(m_fileName) == false)
                throw CException(DataIOExCode::FILE_NOT_EXISTS, "File not exists", __func__, __FILE__, __LINE__);

            if(m_pDataIO)
                return m_pDataIO->read(subset);
            else
                return _DataType();
        }
        _DataType                       readLive()
        {
            if(m_pDataIO)
                return m_pDataIO->readLive();
            else
                return _DataType();
        }
        _DataType                       readLive(const SubsetBounds& subset)
        {
            if(m_pDataIO)
                return m_pDataIO->readLive(subset);
            else
                return _DataType();
        }

        void                            write(const _DataType& data)
        {
            if(m_pDataIO)
                m_pDataIO->write(data);
            else
                throw CException(CoreExCode::NULL_POINTER, "Write error: invalid DataIO object", __func__, __FILE__, __LINE__);
        }
        void                            write(const _DataType& data, const std::string& path)
        {
            if(m_pDataIO)
                m_pDataIO->write(data, path);
            else
                throw CException(CoreExCode::NULL_POINTER, "Write error: invalid DataIO object", __func__, __FILE__, __LINE__);
        }

        void                            stopWrite()
        {
            if(m_pDataIO)
                m_pDataIO->stopWrite();
            else
                throw CException(CoreExCode::NULL_POINTER, "Stop write error: invalid DataIO object", __func__, __FILE__, __LINE__);
        }
        void                            stopRead()
        {
            if(m_pDataIO)
                m_pDataIO->stopRead();
            else
                throw CException(CoreExCode::NULL_POINTER, "Stop read error: invalid DataIO object", __func__, __FILE__, __LINE__);
        }

        void                            waitWriteFinished()
        {
            if(m_pDataIO)
                m_pDataIO->waitWriteFinished();
            else
                throw CException(CoreExCode::NULL_POINTER, "Wait write finished error: invalid DataIO object", __func__, __FILE__, __LINE__);
        }

        void                            closeCamera()
        {
            if(m_pDataIO)
                m_pDataIO->closeCamera();
            else
                throw CException(CoreExCode::NULL_POINTER, "Stop Write error: invalid DataIO object", __func__, __FILE__, __LINE__);
        }

    //private  attributes
    private:

        bool                        m_bNoFileStream = false;
        std::string                 m_fileName;
        std::unique_ptr<_ClassIO>   m_pDataIO = nullptr;
};

#endif // CDATAIO_H
