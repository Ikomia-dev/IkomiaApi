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

#ifndef CEXCEPTION_H
#define CEXCEPTION_H

#include <string>
#include "UtilsGlobal.hpp"
#include "ExceptionCode.hpp"
#include "opencv2/opencv.hpp"

using namespace Ikomia;

/**
 * @brief
 * This class is the base class for exceptions handling into the Ikomia environment.
 * All exceptions generated by the Ikomia API inherits from CException.
 * @ingroup groupUtils
 */
class UTILSSHARED_EXPORT CException : public std::exception
{
    public:

        //Constructors
        /**
         * @brief Default constructor
         */
        CException();             
        /**
         * @brief Constructor
         * @param code Exception code. See ExceptionCode.hpp for details.
         * @param error Exception message
         */
        CException(int code, const std::string &error);
        /**
         * @brief Constructor - The use of this constructor is recommended
         * @param code Exception code. See ExceptionCode.hpp for details.
         * @param error Exception message
         * @param func Function name from where the exception is thrown
         * @param file Source file from where the exception is thrown
         * @param line Line in source file from where the exception is thrown
         */
        CException(int code, const std::string& error, const std::string& func, const std::string& file, int line);
        /**
         * @brief Constructor from OpenCV cv::Exception
         * @param code Exception code. See ExceptionCode.hpp for details.
         * @param e OpenCV exception
         * @param func Function name from where the exception is thrown
         * @param file Source file from where the exception is thrown
         * @param line Line in source file from where the exception is thrown
         */
        CException(int code, const cv::Exception& e, const std::string &func, const std::string &file, int line);

        virtual ~CException();

        virtual const char* what() const noexcept override;

        //For Python binding
        std::string         getMessage() const;
        int                 getCode() const;
        std::string         getError() const;

    private:

        std::string         formatMessage() const;
        std::string         formatFile() const;

    private:

        int         m_code = CoreExCode::UNKNOWN;
        int         m_line = -1;
        int         m_msgType = 0;
        std::string m_error;
        std::string m_func;
        std::string m_file;
        std::string m_msg;
};

#endif // CEXCEPTION_H