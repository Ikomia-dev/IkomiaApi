/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CARRAYIO_H
#define CARRAYIO_H

#include "DataProcessGlobal.hpp"
#include "Data/CMat.hpp"
#include "Protocol/CProtocolTaskIO.h"

/**
 * @ingroup groupDataProcess
 * @brief The CArrayIO class defines an input or output for a workflow task dedicated to multi-dimensional array.
 * @details This class is designed to handle multi-dimensional array as input or output of a workflow task.
 * A CArrayIO instance can be added as input or output to a CProtocolTask or derived object.
 * For Python, data structure is a numpy array.
 */
class CArrayIO : public CProtocolTaskIO
{
    public:

        /**
         * @brief Default constructor
         */
        CArrayIO();
        /**
         * @brief Construct a CArrayIO instance with the given array.
         * @param array: CMat object for C++ and Numpy array for Python.
         */
        CArrayIO(const CMat& array);
        /**
         * @brief Copy constructor.
         */
        CArrayIO(const CArrayIO& io);
        /**
         * @brief Universal reference copy constructor.
         */
        CArrayIO(const CArrayIO&& io);

        virtual ~CArrayIO() = default;

        /**
         * @brief Assignement operator.
         */
        CArrayIO& operator=(const CArrayIO& io);
        /**
         * @brief Universal reference assignement operator.
         */
        CArrayIO& operator=(const CArrayIO&& io);

        /**
         * @brief Sets the array.
         * @param array: CMat object for C++ and Numpy array for Python.
         */
        void            setArray(const CMat& array);
        /**
         * @brief Gets the array data.
         * @return CMat object for C++ and Numpy array for Python.
         */
        CMat            getArray() const;
        /**
         * @brief Gets the array information.
         * @details Information like size and type can be retrieved.
         * @return CDataInfoPtr object or derived.
         */
        CDataInfoPtr    getDataInfo() override;
        /**
         * @brief Gets the number of unit elements when the data is processed.
         * @details The number of unit elements (or atomic) is used to calculate the number of progress steps needed to perform a task.
         * @return Number of unit element.
         */
        size_t          getUnitElementCount() const override;
        /**
         * @brief Checks whether the input/output have valid array or not.
         * @return True if array is not empty, False otherwise.
         */
        bool            isDataAvailable() const override;
        /**
         * @brief Clears image and overlay mask so that they become empty.
         */
        void            clearData() override;
        /**
         * @brief Performs a deep copy the this instance
         * @return CImageProcessIO smart pointer (std::shared_ptr).
         */
        std::shared_ptr<CArrayIO>           clone() const;

    private:

        std::shared_ptr<CProtocolTaskIO>    cloneImp() const override;

    protected:

        CMat            m_array;
};

using ArrayIOPtr = std::shared_ptr<CArrayIO>;

class DATAPROCESSSHARED_EXPORT CArrayIOFactory: public CProtocolTaskIOFactory
{
    public:

        CArrayIOFactory()
        {
            m_name = "CArrayIO";
        }

        virtual ProtocolTaskIOPtr   create(IODataType dataType)
        {
            Q_UNUSED(dataType);
            return std::make_shared<CArrayIO>();
        }
};

#endif // CARRAYIO_H
