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

#ifndef CTASKINFO_H
#define CTASKINFO_H

#include <string>
#include "DataProcessGlobal.hpp"
#include "UtilsDefine.hpp"

using namespace Ikomia;

/**
 * @ingroup groupDataProcess
 * @brief The CTaskInfo class manages metadata associated with a process task.
 * Information are then available for consulting in the software.
 * These metadata are also used by the system search engine (process library and store).
 */
class DATAPROCESSSHARED_EXPORT CTaskInfo
{
    public:

        /**
         * @brief Default constructor.
         */
        CTaskInfo();

        std::string getName() const;
        std::string getPath() const;
        std::string getShortDescription() const;
        std::string getDescription() const;
        std::string getDocumentationLink() const;
        std::string getIconPath() const;
        std::string getKeywords() const;
        std::string getAuthors() const;
        std::string getArticle() const;
        std::string getJournal() const;
        std::string getVersion() const;
        std::string getIkomiaVersion() const;
        std::string getCreatedDate() const;
        std::string getModifiedDate() const;
        std::string getLicense() const;
        std::string getRepository() const;
        int         getYear() const;
        int         getLanguage() const;
        int         getOS() const;

        /** @cond INTERNAL */
        bool        isInternal() const;
        /** @endcond */

        void        setName(const std::string& name);
        void        setPath(const std::string& path);
        void        setShortDescription(const std::string& description);
        void        setDescription(const std::string& description);
        void        setDocumentationLink(const std::string& link);
        void        setIconPath(const std::string& path);
        void        setKeywords(const std::string& keywords);
        void        setAuthors(const std::string& authors);
        void        setArticle(const std::string& article);
        void        setJournal(const std::string& journal);
        void        setVersion(const std::string& version);
        void        setCreatedDate(const std::string& date);
        void        setModifiedDate(const std::string& date);
        void        setLicense(const std::string& license);
        void        setRepository(const std::string& repository);
        void        setYear(const int year);
        void        setLanguage(const int language);
        void        setOS(const int os);

        /** @cond INTERNAL */
        void        setInternal(bool bInternal);
        /** @endcond */

    public:

        /** @cond INTERNAL */
        int         m_id;
        int         m_userId;
        bool        m_bInternal = true;
        /** @endcond */

        std::string m_name = "";                /**< Process task name. Must be unique */
        std::string m_path = "";                /**< Path in the system tree structure of the process library */
        std::string m_shortDescription = "";    /**< Short description of the process */
        std::string m_description = "";         /**< Full description of the process */
        std::string m_docLink = "";             /**< Internet link to an associated documentation page */
        std::string m_iconPath = "";            /**< File path to a custom icon */
        std::string m_keywords = "";            /**< Keywords associated with the process: useful for search engine */
        std::string m_authors = "";             /**< Authors of the process */
        std::string m_article = "";             /**< Associated research article */
        std::string m_journal = "";             /**< Journal of the article */
        std::string m_version = "1.0.0";        /**< Version of the implementation */
        std::string m_ikomiaVersion = "";       /**< Version of the Ikomia APP & API */
        std::string m_createdDate = "";         /**< Date of creation */
        std::string m_modifiedDate = "";        /**< Date of modification */
        std::string m_license = "";
        std::string m_repo = "";
        int         m_year = -1;                /**< Year of the article or the algorithme */
        int         m_language = CPP;           /**< Programming language */
        int         m_os = OSType::LINUX;       /**< Compatible operating system */
};

#endif // CTASKINFO_H
