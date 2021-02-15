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

#ifndef CPROTOCOLTASKPARAM_HPP
#define CPROTOCOLTASKPARAM_HPP

#include <memory>
#include <QString>
#include "Main/CoreGlobal.hpp"
#include "Main/CoreDefine.hpp"

/**
 * @ingroup groupCore
 * @brief
 * This class is the base class for the management of process and plugin parameters.
 * It consists of two virtual methods that should be overriden to fill or get the structure holding parameters value.
 * This structure is used internally by the system to save and load parameters value of process used in a workflow.
 */
class CORESHARED_EXPORT CProtocolTaskParam
{
    public:

        /**
        * @brief Default constructor
        */
        CProtocolTaskParam();
        virtual ~CProtocolTaskParam() = default;

        /**
         * @brief Sets process parameters values from the given map structure.
         * @param Parameters map (UMapString) holding two std::string. Key = parameter name, Value = parameter value.
         */
        virtual void        setParamMap(const UMapString& paramMap);

        /**
         * @brief Gets map structure filled with process parameters values.
         */
        virtual UMapString  getParamMap() const;

        /**
         * @brief Gets hash value from parameters values.
         * The system uses this method de determine if process configuration has changed.
         */
        virtual uint        getHashValue() const;
};

using ProtocolTaskParamPtr = std::shared_ptr<CProtocolTaskParam>;

#endif // CPROTOCOLTASKPARAM_HPP
